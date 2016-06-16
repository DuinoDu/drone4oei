#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>

#include "actuator.h"   			// 控制摄像头的舵机
#include "flyer.h"     	 				// 飞控
#include "imgprocess.hpp" 	// 图像处理算法
//#include "imgsender.h"  		// 发送图片的socket

using boost::asio::ip::tcp;
using namespace std;
using namespace cv;

int camera_id = 0;
Mat frame;

void sendImage(){
    try{
        boost::asio::io_service io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 3200));
        for(;;){
            tcp::socket socket(io);
            acceptor.accept(socket);
            Mat imgSend = Mat::zeros(1, 230400, CV_8U);
            if(frame.isContinuous()){
                imgSend = frame.reshape(0,1);
            }

            boost::system::error_code ec;
            string message((char*) imgSend.data, 230400);
            socket.write_some(boost::asio::buffer(message), ec);
        }
    }
    catch(exception& e){
        cerr << e.what() << endl;
    }
 }


int main( int argc, char **argv )
{
    Actuator actuator;
    Flyer flyer;
    //ImgSender imgsender;

    bool foundRed = false;
    int stepState = 0;

    // open camera
    VideoCapture capture(camera_id);
    if(!capture.isOpened()){ 
        cerr << "Fail to open camera";
        return -1;
    }

    // get image property
    double rate = capture.get(CV_CAP_PROP_FPS);
    int delay = 1000/rate;
    int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);  	// assump the origin is at left top
    int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    int objPosRange = 50;

    //Mat frame;
    Point2d objPoint;

    // begin second thread of sending image
    boost::thread imgSender(sendImage);

    while(true)
    {
        // read image
        if(!capture.read(frame)){
            cerr << "Fail to read image from capture";
            break;
        }
        
        // Step 1: detect red box in the image
        if(flyer.isOK() && (stepState == 0)){ 
            // detect red box
            if(detectRed(frame, objPoint)){ 
                flyer.stopRight();
                flyer.stopLeft();
                foundRed = true;
                stepState = 1;          
            }
            else{ 
                if(whereIsRed(frame)){
                    flyer.moveRight();
                    continue;
                }    
                else{ 
                    flyer.moveLeft();
                    continue;
                }
            }           
        }
        else
            continue;

        // for each frame, keep red box in the image
        if(!detectRed(frame, objPoint)){ 
            foundRed = false;
            stepState = 0;
            continue;
        }
       
        // Step 2: keep red box at the horizontal center
        if(foundRed && (stepState == 1)){ 
            if(objPoint.x > (width/2 + objPosRange)){ 
                flyer.moveRight();
                continue;
            }            
            else if(objPoint.x < (width/2 - objPosRange)){ 
                flyer.moveLeft();
                continue;
            }
            else{ 
                flyer.stopRight(); // x = width/2
                flyer.stopLeft();
                stepState = 2;
            }
        }

        // Step 3: keep red box at the vertical center (PID?)
        if(foundRed && (stepState == 2)){

            if(objPoint.y > (height/2 + objPosRange)){ 
                actuator.moveUp(); //　假设舵机是给一条指令，转动一个角度，缺点是，舵机会抖到不行
                continue;
            }            
            else if(objPoint.y < (height/2 - objPosRange)){ 
                actuator.moveDown();
                continue;
            }
            else{
                actuator.stopDown();
                //actuator.stopUp();
                stepState = 3;
            }
        }

        // Step 4: move toward the red box. Keep red box in the image center
        if(foundRed && (stepState == 3)){ 
            flyer.moveForward(); // 多次发送会不会有影响？？？

            if(objPoint.y > (height/2 + objPosRange)){
                actuator.moveUp(); //　假设舵机是给一条指令，转动一个角度，缺点是，舵机会抖到不行
            }
            else if(objPoint.y < (height/2 - objPosRange)){
                actuator.moveDown();
            }
            else{
                actuator.stopDown();
            }

           // if arrive at the red box
           if (actuator.getAngle() > 85){
                stepState = 4;
                flyer.stopForward();
                flyer.arriving();
           }
        }
        
        /*
        
        // 到达指定目标后，飞控执行以下操作：
        // １、下降
        // ２、扔球
        // ３、上升
        // ４、转身// or turn camera back

        */

        // 寻找起始点目标，相同逻辑，返航。

        if (flyer.finishThrow && stepState == 4){

        }
    }
    return 0;
}

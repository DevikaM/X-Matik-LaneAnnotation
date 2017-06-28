#include "video.h"
#include <QtCore>

using namespace cv;


Video::Video(){}

Video::Video(std::string path){}

Video::Video(std::string path, std::string outputDir, std::string File, int FrameRate,MainWindow* window){
    cv::VideoCapture cap(path);
    if(cap.isOpened()){
        try{
            int i = atoi(File.c_str());
            //MainWindow* window = new MainWindow();

            CVImage* imageWidget = new CVImage();

            int total_fr = (int)cap.get (CV_CAP_PROP_FRAME_COUNT);
            int index= 0;



            
            for(int f =0; f < total_fr; f++)
            {
                if(index == 0){
                    Mat fr;
                    cap.set(CAP_PROP_POS_FRAMES,f)  ;
                    cap.grab();
                    cap.retrieve(fr);

                    std::string filePath = outputDir;
                    std::string fileName = std::to_string(i)+".png";
                    cv::imwrite(filePath+fileName, fr);
                    //Mat fram = imread(filePath+fileName);
                    //imwrite(filePath+fileName, fram);
                    imageWidget->showImage(fr);
                    window->setImage(imageWidget);

                    i++;
                    //std::cin.clear();
                    qDebug()<< i << "-----------------------------";
                }else if(index == FrameRate){
                    index = -1;
                }
                
                index++;

            }
            
           /*for(;;){
                cv::Mat fr;
                cap >> fr;
                std::string filePath = outputDir + "all\\";
                std::string fileName = std::to_string(i)+".png";
               // std::cout << filePath+fileName << std::endl;
                cv::imwrite(filePath+fileName, fr);

                Frame f(filePath+fileName, outputDir, fileName);
                f.run();
                i++;
            }*/
           // cv::destroyWindow("image");
            }
        catch(cv::Exception e){
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Exception has been catched");
            messageBox.setFixedSize(500,200);
            exit(0);
        }
   }
}

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Mat F1 = imread("I1.png", CV_LOAD_IMAGE_GRAYSCALE); //read the image data in the file "MyPic.JPG" and store it in 'F1'

     if (F1.empty()) //check whether the image is loaded or not
     {
          cout << "Error : Image cannot be loaded..!!" << endl;
          //system("pause"); //wait for a key press
          return -1;
     }

     Mat F2=F1.clone();
     int temp[F1.rows-2][F1.cols-2];
     int min=INT_MAX;
     int max=INT_MIN;
     int kernel[3][3]={{0,-1,0},{-1,4,-1},{0,-1,0}};
     int sum=0;
     Scalar colour;

     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            for(int k=i-1;k<=i+1;k++)
            {
               for(int l=j-1;l<=j+1;l++)
               {
                  colour=F1.at<uchar>(Point(k,l));
                  sum+=colour.val[0]*kernel[k-i+1][l-j+1];
               }
            }
            temp[i-1][j-1]=sum;
            if(max<sum)max=sum;
            if(min>sum)min=sum;
            sum=0;
        }
     }
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            F2.at<uchar>(Point(i,j))=int((temp[i-1][j-1]-min)*255/(max-min));
        }
     }

     min=INT_MAX;
     max=INT_MIN;
     int temp2[F1.rows][F1.cols]; 
     Mat F3=F1.clone();
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            temp2[i][j]=F2.at<uchar>(Point(i,j))+F1.at<uchar>(Point(i,j));
            if(max<temp2[i][j])max=temp2[i][j];
            if(min>temp2[i][j])min=temp2[i][j];
        }
     }
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            F3.at<uchar>(Point(i,j))=int((temp2[i][j]-min)*255/(max-min));
        }
     }

     min=INT_MAX;
     max=INT_MIN;
     Mat F4=F1.clone();
     int sumx=0, sumy=0;
     int sobelx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
     int sobely[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
     sum=0;
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            for(int k=i-1;k<=i+1;k++)
            {
               for(int l=j-1;l<=j+1;l++)
               {
                  colour=F1.at<uchar>(Point(k,l));
                  sumx+=colour.val[0]*sobelx[k-i+1][l-j+1];
                  sumy+=colour.val[0]*sobely[k-i+1][l-j+1];
               }
            }
            sum=sqrt(pow(sumx, 2)+pow(sumy,2));
            temp2[i][j]=sum;
            if(max<sum)max=sum;
            if(min>sum)min=sum;
            sumx=0;
            sumy=0;
            sum=0;
        }
     }

     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            sum=int((temp2[i][j]-min)*255/(max-min));
            F4.at<uchar>(Point(i,j))=sum;
        }
     }
     int smoothfilter[3][3]={{1,1,1},{1,1,1},{1,1,1}};

     min=INT_MAX;
     max=INT_MIN;
     sum=0;
     Mat F5=F1.clone();
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            for(int k=i-1;k<=i+1;k++)
            {
               for(int l=j-1;l<=j+1;l++)
               {
                  colour=F4.at<uchar>(Point(k,l));
                  sum+=colour.val[0]*smoothfilter[k-i+1][l-j+1];
               }
            }
            sum/=9;
            F5.at<uchar>(Point(i,j))=sum;
            sum=0;
        }
     }

     min=INT_MAX;
     max=INT_MIN;
     sum=0;
     Mat F6=F1.clone();
     Scalar colour2;
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            colour=F3.at<uchar>(Point(i,j));
            colour2=F5.at<uchar>(Point(i,j));
            sum=colour.val[0]*colour2.val[0];
            F6.at<uchar>(Point(i,j))=sum/255;
        }
     }

     Mat F7=F1.clone();

     min=INT_MAX;
     max=INT_MIN;
     for(int i=1;i<F1.rows-1;i++)
     {
        for(int j=1;j<F1.cols-1;j++)
        {
            sum=F1.at<uchar>(Point(i,j))+1.7*F6.at<uchar>(Point(i,j));
            F7.at<uchar>(Point(i,j))=sum>255?255:sum;
        }
     }

     namedWindow("1MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("1MyWindow", F1); //display the image which is stored in the 'F1' in the "MyWindow" window
     namedWindow("2MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("2MyWindow", F2); //display the image which is stored in the 'F1' in the "MyWindow" window
     namedWindow("3MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("3MyWindow", F3); //display the image which is stored in the 'F1' in the "MyWindow" window
     namedWindow("4MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("4MyWindow", F4); //display the image which is stored in the 'F1' in the "MyWindow" window
     namedWindow("5MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("5MyWindow", F5); //display the image which is stored in the 'F1' in the "MyWindow" window
     namedWindow("6MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("6MyWindow", F6); //display the image which is stored in the 'F1' in the "MyWindow" window
     namedWindow("7MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("7MyWindow", F7); //display the image which is stored in the 'F1' in the "MyWindow" window

     waitKey(0); //wait infinite time for a keypress

     destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"

    return 0;

}
#include <cv.h>
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>

using namespace cv;
using namespace std;

struct Circle{
	int radius;
	int cx;
	int cy;
	Circle(int r, int x, int y): radius(r), cx(x), cy(y) {}
};

Mat getGrayscale(Mat src)
{
	Mat dst(src.rows, src.cols, CV_8UC1);
	dst = Scalar(0);
	Vec3b colour;
	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
				colour=src.at<Vec3b>(Point(j,i));
				int r=(int)colour.val[0];
				int g=(int)colour.val[1];
				int b=(int)colour.val[2];
				dst.at<uchar>(Point(j,i))=r*0.1+g*0.6+b*0.3;		
		}	
	}
	return dst;
}

Mat getHistEQ(Mat src)
{
	Mat dst=src.clone();
	Scalar colour;	
	vector<int> hist(256,0);
	vector<double> cprob(256,0);
	vector<int> cumf(256,0);
	vector<int> histeq(256,0);
	int tot_pix=src.rows*src.cols;
	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			colour=src.at<uchar>(Point(j,i));
			hist[colour.val[0]]++;
		}
	}
	int sum=0;

	for(int i=0;i<256;i++)
	{
		sum+=hist[i];
		cumf[i]=sum;
		cprob[i]=(double)cumf[i]/tot_pix;
		histeq[i]=cprob[i]*255;
	}

	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			colour=src.at<uchar>(Point(j,i));
			dst.at<uchar>(Point(j,i))=histeq[colour.val[0]];	
		}
	}

	return dst;
}

Mat getSobel(Mat src)
{
	Mat dst=src.clone();
	Scalar colour;	
	int min=INT_MAX;
	int max=INT_MIN;
	int sumx=0, sumy=0;
	int temp[src.rows][src.cols];
	int sobelx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
	int sobely[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
	int sum=0;
	for(int i=1;i<src.rows-1;i++)
	{
		for(int j=1;j<src.cols-1;j++)
		{
			for(int k=i-1;k<=i+1;k++)
			{
				for(int l=j-1;l<=j+1;l++)
				{
					colour=src.at<uchar>(Point(l,k));
					sumx+=colour.val[0]*sobelx[k-i+1][l-j+1];
					sumy+=colour.val[0]*sobely[k-i+1][l-j+1];
				}
			}
			sum=sqrt(pow(sumx, 2)+pow(sumy,2));
			temp[i][j]=sum;
			if(max<sum)max=sum;
			if(min>sum)min=sum;
			sumx=0;
			sumy=0;
			sum=0;
		}
	}
	for(int i=1;i<dst.rows-1;i++)
	{
		for(int j=1;j<dst.cols-1;j++)
		{
			sum=int((temp[i][j]-min)*255/(max-min));
			dst.at<uchar>(Point(j,i))=sum;
		}
	}

	return dst;
}

Mat getBackground(Mat src)
{
	Mat dst(src.rows, src.cols, CV_8UC1);
	Vec3b colour;
	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			colour=src.at<Vec3b>(Point(j,i));
			if(colour.val[0]>10||colour.val[1]>10||colour.val[2]>10)
				dst.at<uchar>(Point(j,i))=255;
			else
				dst.at<uchar>(Point(j,i))=0;
		}
	}

	return dst;
}

Mat getBinary(Mat src)
{
	Mat dst=src.clone();
	Scalar colour;
	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			colour=src.at<uchar>(Point(j,i));
			if(colour.val[0]<20)
				dst.at<uchar>(Point(j,i))=0;
			else
				dst.at<uchar>(Point(j,i))=255;
		}
	}
	return dst;
}

Mat getEroded(Mat src)
{
	Mat dst(src.rows, src.cols, CV_8UC1);
	for(int i=0;i<src.rows;++i)
	{
		for(int j=0;j<src.cols;++j)
		{
			int k,l,black=0,white=0;
			for(k=i-3;k<=i+3;++k)
			{
				for(l=j-3;l<=j+3;++l)
				{
					if(k>=0&&k<src.rows&&l>=0&&l<src.cols)
					{
						if(src.at<uchar>(Point(l,k))<10)
							black++;
						else
							white++;
					}
				}
			}
			if(white<black)
				dst.at<uchar>(i,j)=255;
			else
				dst.at<uchar>(i,j)=0;
		}
	}
	return dst;
}

int checkDefective(Mat src)
{
	Scalar colour;
	int count=0;
	src=getEroded(src);
	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			colour=src.at<uchar>(Point(j,i));
			// cout<<colour.val[0]<<" ";
			if(colour.val[0]<10)count++;
		}
	}
	if(count>10)return 1;
	return 0;
}

Mat getWithoutCandies(Mat src, Mat ori)
{
	Mat dst(src.rows, src.cols, CV_8UC1);
	Scalar colour, colour2;
	for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			colour=src.at<uchar>(Point(j,i));
			colour2=ori.at<uchar>(Point(j,i));
			if(colour.val[0]<10&&colour2.val[0]>10)
				dst.at<uchar>(Point(j,i))=255;
			else
				dst.at<uchar>(Point(j,i))=0;
		}
	}

	return dst;
}

int main()
{
	Mat col=imread("J3.png"); // The original image
	cvtColor(col, col, CV_BGRA2RGB);
	Mat ori, histeq, edges, binary, background, remove_candies;
	vector<Circle> candy_list;
	vector<vector<int>  > hough(col.rows, vector<int> (col.cols, 0));
	map<string, int> counts;
	// Detect edges in the image
	ori=getGrayscale(col);
	// histeq=getHistEQ(ori);
	edges=getSobel(ori);
	binary=getBinary(edges);

	// namedWindow("Binary", CV_WINDOW_AUTOSIZE);
	// imshow("Binary", binary);
	// namedWindow("Sobel", CV_WINDOW_AUTOSIZE);
	// imshow("Sobel", edges);
	// namedWindow("HistEQ", CV_WINDOW_AUTOSIZE);
	// imshow("HistEQ", histeq);
	// namedWindow("Original", CV_WINDOW_AUTOSIZE);
	// imshow("Original", ori);
	// waitKey(0);
	int min_r, max_r;
	cout<<"Enter minimum expected Radius:";
	cin>>min_r;
	cout<<"Enter maximum expected Radius:";
	cin>>max_r;


	Mat candies(ori.rows, ori.cols, CV_8UC3);
	candies = Scalar(0,0,0);
	Mat mask(ori.rows, ori.cols, CV_8UC3);
	mask = Scalar(0,0,0);
	// Loop through different values of the radiusmin(binary.rows, binary.cols)/2
	for(int radius = min_r; radius<max_r; radius++)
	{
		cout<<radius<<"\n";
		// Loop through each pixel
		for(int x=0;x<binary.rows;x++)
		{
			for(int y=0;y<binary.cols;y++)
			{
				// Check if the current pixel is an edge
				int value = binary.at<uchar>(Point(y,x));
				if(value==0) continue;
			
				// If it is an edge, generate its circle in the parameter space
				for(double theta=0;theta<360;theta=theta+1)
				{
					int a = x + radius*cos(theta*3.1412/180);
					int b = y + radius*sin(theta*3.1412/180);

					// Just in case
					if(a<0 || a>=binary.rows || b<0 || b>=binary.cols) continue;
					hough[a][b]++;
				}
			}
		}

		// cout<<radius<<" ";
		for(int x=0;x<binary.rows;x++)
		{
			for(int y=0;y<binary.cols;y++)
			{
				if(hough[x][y]>200){
					// cout<<x<<" "<<y<<"\n";
					int there=0;
					for(int j=0; j<candy_list.size();j++)
					{
						Circle c=candy_list[j];
						if(sqrt(pow(radius-c.radius,2)
								+ pow(x-c.cx,2)+pow(y-c.cy,2))<10.0){

							candy_list[j].radius=(c.radius+radius)/2;
							candy_list[j].cx=(c.cx+x)/2;
							candy_list[j].cy=(c.cy+y)/2;
							there=1;
							break;
						}
					}
					if(!there){
						candy_list.push_back(Circle(radius, x, y));
					}
				}
				// cout<<hough[x][y]<<" ";
				hough[x][y]=0;
			}
		}

		// waitKey(0);

	}


	string clr="";
	cout<<"====================================================================\n";

	for(int i=0;i<candy_list.size();i++)
	{
		candies=Scalar(0,0,0);
		Circle c=candy_list[i];
		cout<<"Radius:"<<c.radius<<" X:"<<c.cx<<" Y:"<<c.cy<<"\n";
		circle(candies, Point(c.cy,c.cx), c.radius, Scalar(col.at<Vec3b>(Point(c.cy,c.cx))), -5, 8, 0);
		circle(mask, Point(c.cy,c.cx), c.radius, Scalar(col.at<Vec3b>(Point(c.cy,c.cx))), -5, 8, 0);
		circle(mask, Point(c.cy,c.cx), c.radius, Scalar(col.at<Vec3b>(Point(c.cy,c.cx))), 6, 8, 0);
		Vec3b pix=col.at<Vec3b>(Point(c.cy,c.cx));

		clr=to_string(pix.val[0])+" "+to_string(pix.val[1])+" "+to_string(pix.val[2]);
		if(counts.find(clr)==counts.end())
			counts[clr]=1;
		else counts[clr]++;

		namedWindow("Candies", CV_WINDOW_AUTOSIZE);
		imshow("Candies", candies);
		cout<<"Press enter on image window to see next detected candy\n";
		waitKey(0);
	}

	map<string, int>::iterator it;


	background = getBackground(mask);
	remove_candies = getWithoutCandies(background, binary);
	remove_candies=getBinary(remove_candies);
	namedWindow("Only Candies [White]", CV_WINDOW_AUTOSIZE);
	imshow("Only Candies [White]", background); 

	namedWindow("Background", CV_WINDOW_AUTOSIZE);
	imshow("Background", remove_candies); 

	cout<<"====================================================================\n\n";
	for(it=counts.begin();it!=counts.end();it++)
	{
		cout<<"Colour:"<<it->first<<":"<<it->second<<"\n";
	}

	cout<<"====================================================================\n\n";
	cout<<"Number of Candies:"<<candy_list.size()<<"\n\n";
	cout<<"====================================================================\n";


	cout<<"====================================================================\n\n";
	if(checkDefective(remove_candies))
	{
		cout<<"Image contains defective candies. Please correct the system.\n\n";
	}
	else {
		cout<<"No defective candies found.\n\n";
	}
	cout<<"====================================================================\n";
	waitKey(0);

	return 0;
}
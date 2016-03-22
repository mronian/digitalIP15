Submission by : Anchit Navelkar (12CS30004)

To compile : 

g++ -ggdb `pkg-config --cflags opencv` ass2_12CS30004.cpp `pkg-config --libs opencv` -std=c++11

Procedure Used :

1. Get Input
2. Threshold
3. Sobel Edge
4. Binarise
5. Find Hough Circles
6. Background Subtraction
7. Find Defective Candies
8. Print results

Once candies are found, keep pressing enter for each separate candy. The output also shows radius and centre of the candy found. Once all candies are shown, the code checks for defective candies and outputs the background.
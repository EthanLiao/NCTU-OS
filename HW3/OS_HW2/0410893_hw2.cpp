#include "bmpReader.h"
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <cstdlib>
#include <iostream>
using namespace std;

BmpImage input_pic;
const int BILINEAR_RATIO = 7;
float ROTATION_ANGLE = 45;
float ROTATION_ANGLE_2 = 60;
float SHEAR_VERTICAL_DEGREE_1 = 0.3;
float SHEAR_VERTICAL_DEGREE = 0.45;
float SHEAR_HORIZONTAL_DEGREE_1 = 0.6;
float SHEAR_HORIZONTAL_DEGREE = 0.45;
BmpImage global_pic;
BmpReader* bmpReader;

const char* INPUT_IMAGE = "input.bmp";
const char* OUT_IMAGE[5] = {"output1.bmp", "output2.bmp", "output3.bmp", "output4.bmp", "output5.bmp"};

BmpImage bilinear(BmpImage pic, int nw, int nh);
BmpImage rotation(BmpImage pic, int nw,int nh);
BmpImage shear_vertical(BmpImage pic, int nw, int nh);
BmpImage shear_horizontal(BmpImage pic, int nw, int nh);
BmpImage shear_horizontal_2(BmpImage pic, int nw, int nh);
BmpImage shear_vertical_2(BmpImage pic, int nw, int nh);
BmpImage rotation_2(BmpImage pic, int nw,int nh);

void *process1(void *ptr)
{
	int *q;
	q=(int*)ptr;
	BmpReader* bmpReader0 = bmpReader;
	BmpImage process1_pic=global_pic;
	int newWidth1 = (int)(process1_pic.width * BILINEAR_RATIO);
	int newHeight1 = (int)(process1_pic.height * BILINEAR_RATIO);
	BmpImage bilinear_pic1 = bilinear(process1_pic, newWidth1, newHeight1);
	//Write bmp files
	bmpReader0->WriteBMP(OUT_IMAGE[0], bilinear_pic1);
	pthread_exit(NULL);
}

void *process2(void *ptr)
{
	int *q;
	q=(int*)ptr;
	BmpReader* bmpReader1 = bmpReader;
	BmpImage process2_pic=global_pic;
	int newWidth2 = (int)(process2_pic.width * BILINEAR_RATIO);
	int newHeight2 = (int)(process2_pic.height * BILINEAR_RATIO);
	BmpImage bilinear_pic2 = bilinear(process2_pic, newWidth2, newHeight2);
	BmpImage rotation_pic2 = rotation(bilinear_pic2, newWidth2, newHeight2);
	//Write bmp files
	bmpReader1->WriteBMP(OUT_IMAGE[1], rotation_pic2);
	pthread_exit(NULL);
}

void *process3(void *ptr)
{
	int *q;
	q=(int*)ptr;
	BmpReader* bmpReader2 = bmpReader;
	BmpImage process3_pic=global_pic;
	int newWidth3 = (int)(process3_pic.width * BILINEAR_RATIO);
	int newHeight3 = (int)(process3_pic.height * BILINEAR_RATIO);
	BmpImage bilinear_pic3 = bilinear(process3_pic, newWidth3, newHeight3);
	BmpImage shearVert_pic3 = shear_vertical(bilinear_pic3, newWidth3, newHeight3);
	bmpReader2->WriteBMP(OUT_IMAGE[2], shearVert_pic3);
	pthread_exit(NULL);
}

void *process4(void *ptr)
{
	int *q;
	q=(int*)ptr;
	BmpReader* bmpReader3 = bmpReader;
	BmpImage process4_pic=global_pic;
	int newWidth4 = (int)(process4_pic.width * BILINEAR_RATIO);
	int newHeight4 = (int)(process4_pic.height * BILINEAR_RATIO);
	BmpImage bilinear_pic4 = bilinear(process4_pic, newWidth4, newHeight4);
	BmpImage shearHori_pic4 = shear_horizontal(bilinear_pic4, newWidth4, newHeight4);
	bmpReader3->WriteBMP(OUT_IMAGE[3], shearHori_pic4);
	pthread_exit(NULL);
}

void *process5(void *ptr)
{
	int *q;
	q=(int*)ptr;
	BmpReader* bmpReader4 = bmpReader;
	BmpImage process5_pic=global_pic;
	int newWidth5 = (int)(process5_pic.width * BILINEAR_RATIO);
	int newHeight5 = (int)(process5_pic.height * BILINEAR_RATIO);
	BmpImage bilinear_pic5 = bilinear(process5_pic, newWidth5, newHeight5);
	BmpImage shearVert_pic_5 = shear_vertical_2(bilinear_pic5, newWidth5, newHeight5);
	BmpImage tmp_pic5 = rotation_2(shearVert_pic_5, newWidth5, newHeight5);
	BmpImage mix_pic5 = shear_horizontal_2(tmp_pic5, newWidth5, newHeight5);
	bmpReader4->WriteBMP(OUT_IMAGE[4], mix_pic5);
	pthread_exit(NULL);
}




// TODO: add your pthread codes to speed up the program
/*void* runner(void *in)
{
	BmpReader* bmpReader = new BmpReader();
	//BmpImage input_pic = bmpReader->ReadBMP(argv[1]);

	int newWidth = (int)(input_pic.width * BILINEAR_RATIO);
	int newHeight = (int)(input_pic.height * BILINEAR_RATIO);

	//all image processing
	BmpImage bilinear_pic = bilinear(input_pic, newWidth, newHeight);
	BmpImage rotation_pic = rotation(bilinear_pic, newWidth, newHeight);
	BmpImage shearVert_pic = shear_vertical(bilinear_pic, newWidth, newHeight);
	BmpImage shearHori_pic = shear_horizontal(bilinear_pic, newWidth, newHeight);

	BmpImage shearVert_pic_2 = shear_vertical_2(bilinear_pic, newWidth, newHeight);
	BmpImage tmp_pic = rotation_2(shearVert_pic_2, newWidth, newHeight);
	BmpImage mix_pic = shear_horizontal_2(tmp_pic, newWidth, newHeight);

	//Write bmp files
	bmpReader->WriteBMP(OUT_IMAGE[0], bilinear_pic);
	bmpReader->WriteBMP(OUT_IMAGE[1], rotation_pic);
	bmpReader->WriteBMP(OUT_IMAGE[2], shearVert_pic);
	bmpReader->WriteBMP(OUT_IMAGE[3], shearHori_pic);
	bmpReader->WriteBMP(OUT_IMAGE[4], mix_pic);

	//free memory
	free(input_pic.data);
	free(bilinear_pic.data);
	free(rotation_pic.data);
	free(shearVert_pic.data);
	free(shearHori_pic.data);
	free(tmp_pic.data);
	free(mix_pic.data);

    pthread_exit((void*)0);
}*/

int main(int argc,char *argv[]){


	struct timeval  tv1, tv2;
	// timing function 1
/*Single thread*/
	gettimeofday(&tv1, NULL);
	bmpReader = new BmpReader();
	BmpImage input_pic = bmpReader->ReadBMP(argv[1]);
	global_pic=input_pic;

	int newWidth = (int)(input_pic.width * BILINEAR_RATIO);
	int newHeight = (int)(input_pic.height * BILINEAR_RATIO);

	//all image processing
	BmpImage bilinear_pic = bilinear(input_pic, newWidth, newHeight);
	BmpImage rotation_pic = rotation(bilinear_pic, newWidth, newHeight);
	BmpImage shearVert_pic = shear_vertical(bilinear_pic, newWidth, newHeight);
	BmpImage shearHori_pic = shear_horizontal(bilinear_pic, newWidth, newHeight);

	BmpImage shearVert_pic_2 = shear_vertical_2(bilinear_pic, newWidth, newHeight);
	BmpImage tmp_pic = rotation_2(shearVert_pic_2, newWidth, newHeight);
	BmpImage mix_pic = shear_horizontal_2(tmp_pic, newWidth, newHeight);

	//Write bmp files
	bmpReader->WriteBMP(OUT_IMAGE[0], bilinear_pic);
	bmpReader->WriteBMP(OUT_IMAGE[1], rotation_pic);
	bmpReader->WriteBMP(OUT_IMAGE[2], shearVert_pic);
	bmpReader->WriteBMP(OUT_IMAGE[3], shearHori_pic);
	bmpReader->WriteBMP(OUT_IMAGE[4], mix_pic);
	gettimeofday(&tv2, NULL);
	//free memory
	free(input_pic.data);
	free(bilinear_pic.data);
	free(rotation_pic.data);
	free(shearVert_pic.data);
	free(shearHori_pic.data);
	free(tmp_pic.data);
	free(mix_pic.data);
	// timing function 2

	cout<<"Without using pthread : "<<"\n";
	printf("0410893\n %f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
	double time1=(double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);

/*Multi thread*/
	// TODO: modify (ID) to your student ID
	/* int id[16];
	for(int i=0;i<16;i++) id[i]=i;
	gettimeofday(&tv1, NULL);
	pthread_t tid[16];
	pthread_attr_t attr;
  pthread_attr_init(&attr);
	for(int i=0;i<16;i++)
	{
		if(pthread_create(tid+i,&attr,runner,id+i)!=0)
			{
				perror("pthread");
				exit(EXIT_FAILURE);
			}
	}
	pthread_attr_destroy(&attr);*/
	int k=0;
	gettimeofday(&tv1, NULL);
	pthread_t tid[5];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&tid[0], &attr, process1, (void *)&k);
  pthread_create(&tid[1], NULL, process2, (void *)&k);
  pthread_create(&tid[2], NULL, process3, (void *)&k);
  pthread_create(&tid[3], NULL, process4, (void *)&k);
  pthread_create(&tid[4], NULL, process5, (void *)&k);
	pthread_attr_destroy(&attr);

  void* status;

	for(int i = 0; i < 5; i++ )
	{
		int c = pthread_join(tid[i], &status);
		if (c)
		{
			 cout << "Error:unable to join," << c << endl;
			 exit(-1);
		}
	}

	gettimeofday(&tv2, NULL);
	cout<<"Using pthread : "<<"\n";
	double time2=(double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	printf("0410893\n %f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));

	// TODO: add your pthread codes to speed up the program
	cout<<"\nspeed up rate\n"<<time1/time2;
	pthread_exit(NULL);
	return 0;
}


BmpImage bilinear(BmpImage pic, int newWidth, int newHeight)
{
	unsigned char* data	= (unsigned char*)malloc(3 * newWidth * newHeight * sizeof(unsigned char));

	for(int i = 0 ; i < newWidth; i++){
		for(int j = 0;j < newHeight; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;
				int relevant_w = (int)(i / BILINEAR_RATIO);
				int relevant_h = (int)(j / BILINEAR_RATIO);
				int reference_points[4];

				// the four reference points of this pixel
				reference_points[0] = pic.data[3 * (relevant_h * pic.width + relevant_w) + k];
				reference_points[1] = pic.data[3 * (relevant_h * pic.width + (relevant_w + 1)) + k];
				reference_points[2] = pic.data[3 * ((relevant_h + 1) * pic.width + relevant_w) + k];
				reference_points[3] = pic.data[3 * ((relevant_h + 1) * pic.width + (relevant_w + 1)) + k];

        pixel=0.25*(reference_points[0]+reference_points[1]+reference_points[2]+reference_points[3]);

				if (pixel < 0) pixel = 0;
				if (pixel > 255) pixel = 255;

				data[3 * (j * newWidth + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = newWidth;
	newBmpImage.height = newHeight;

	return newBmpImage;
}

BmpImage rotation(BmpImage pic,int nw,int nh)
{
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	int w_offset = (int)(nw / 2);
	int h_offset = (int)(nh / 2);

	for(int i = 0 ; i < nw; i++){
		for(int j = 0; j < nh; j++){
			for(int k = 0 ; k < 3 ; k++){

				double angle = (double)ROTATION_ANGLE * M_PI / 180.0;
				double cos_theta = cos(angle);
				double sin_theta = sin(angle);
				int relevant_w = i - w_offset;
				int relevant_h = j - h_offset;
				int trans_w = (int)(relevant_w * cos_theta - relevant_h * sin_theta) + w_offset;
				int trans_h = (int)(relevant_h * cos_theta + relevant_w * sin_theta) + h_offset;

				int pixel;

				if (trans_w >= 0 && trans_w < w_offset * 2 && trans_h >= 0 && trans_h < h_offset * 2){
					pixel = pic.data[3 * (trans_h * 2 * w_offset + trans_w) + k];
				}else{
					pixel = 0;
				}

				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}


BmpImage shear_vertical(BmpImage pic, int nw, int nh)
{
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;
				int h_offset = (int)(nw * SHEAR_VERTICAL_DEGREE_1 / 2);
				int trans_w = i;
				int trans_h = (int)(j + i * SHEAR_VERTICAL_DEGREE_1) - h_offset;

				if (trans_h >= 0 && trans_h < nh){
					pixel = pic.data[3 * (trans_h * nw + trans_w) + k];
				}else{
					pixel = 0;
				}

				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}

BmpImage shear_horizontal(BmpImage pic, int nw, int nh)
{
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;

				// TODO: refer to shear_vertical, try to write shear_horizontal on your own
				int w_offset = (int)(nh * SHEAR_HORIZONTAL_DEGREE_1/2);
				int trans_w = (int)(i + j * SHEAR_HORIZONTAL_DEGREE_1) - w_offset;
				int trans_h = j;

				if (trans_w >= 0 && trans_w < nw){
					pixel = pic.data[3 * (trans_h * nw + trans_w) + k];
				}else{
					pixel = 0;
				}

				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}


BmpImage shear_horizontal_2(BmpImage pic, int nw, int nh)
{
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;

				// TODO: refer to shear_vertical, try to write shear_horizontal on your own
				int w_offset = (int)(nh * SHEAR_HORIZONTAL_DEGREE/2);
				int trans_w = (int)(i + j * SHEAR_HORIZONTAL_DEGREE) - w_offset;
				int trans_h = j;

				if (trans_w >= 0 && trans_w < nw){
					pixel = pic.data[3 * (trans_h * nw + trans_w) + k];
				}else{
					pixel = 0;
				}

				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}

BmpImage rotation_2(BmpImage pic,int nw,int nh)
{
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	int w_offset = (int)(nw / 2);
	int h_offset = (int)(nh / 2);

	for(int i = 0 ; i < nw; i++){
		for(int j = 0; j < nh; j++){
			for(int k = 0 ; k < 3 ; k++){

				double angle = (double)ROTATION_ANGLE_2 * M_PI / 180.0;
				double cos_theta = cos(angle);
				double sin_theta = sin(angle);
				int relevant_w = i - w_offset;
				int relevant_h = j - h_offset;
				int trans_w = (int)(relevant_w * cos_theta - relevant_h * sin_theta) + w_offset;
				int trans_h = (int)(relevant_h * cos_theta + relevant_w * sin_theta) + h_offset;

				int pixel;

				if (trans_w >= 0 && trans_w < w_offset * 2 && trans_h >= 0 && trans_h < h_offset * 2){
					pixel = pic.data[3 * (trans_h * 2 * w_offset + trans_w) + k];
				}else{
					pixel = 0;
				}

				data[3 * (j * nw + i) + k] = pixel;
			}
		}
	}

	BmpImage newBmpImage;

	newBmpImage.data = data;
	newBmpImage.width = nw;
	newBmpImage.height = nh;

	return newBmpImage;
}


BmpImage shear_vertical_2(BmpImage pic, int nw, int nh)
{

		unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

		for(int i = 0 ; i < nw ; i++){
			for(int j = 0 ; j < nh ; j++){
				//k is RGB color, R = 2, G = 1, B = 0
				for(int k = 0;k < 3;k++){
					int pixel = 0;
					int h_offset = (int)(nw * SHEAR_VERTICAL_DEGREE /2);
					int trans_w = i;
					int trans_h = (int)(j + i * SHEAR_VERTICAL_DEGREE) - h_offset;

					if (trans_h >= 0 && trans_h < nh){
						pixel = pic.data[3 * (trans_h * nw + trans_w) + k];
					}else{
						pixel = 0;
					}

					data[3 * (j * nw + i) + k] = pixel;
				}
			}
		}

		BmpImage newBmpImage;

		newBmpImage.data = data;
		newBmpImage.width = nw;
		newBmpImage.height = nh;

		return newBmpImage;
}

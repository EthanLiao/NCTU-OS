#include "bmpReader.h"
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>
#define NUM_THREADS 5
using namespace std;
const int BILINEAR_RATIO = 7;

//float ROTATION_ANGLE = 45;
//float SHEAR_VERTICAL_DEGREE = 0.3;
//float SHEAR_HORIZONTAL_DEGREE = 0.6;

const char* INPUT_IMAGE = "input.bmp";
const char* OUT_IMAGE[5] = {"output1.bmp", "output2.bmp", "output3.bmp", "output4.bmp", "output5.bmp"};
int rc;
float time_elapse_first, time_elapse_second;
BmpImage bilinear(BmpImage pic, int nw, int nh);
BmpImage rotation(BmpImage pic, int nw,int nh,int rot);
BmpImage shear_vertical(BmpImage pic, int nw, int nh,float ang);
BmpImage shear_horizontal(BmpImage pic, int nw, int nh,float ang);

void *work_one(void *threadid);
void *work_two(void *threadid);
void *work_three(void *threadid);
void *work_four(void *threadid);
void *work_five(void *threadid);

struct bilinear_data {
	BmpImage pic;
	int nw;
	int nh;
	BmpReader* bmpReader;
};

struct rotation_data {
	BmpImage pic;
	int nw;
	int nh;
	int rot;
	BmpReader* bmpReader;
};

struct shear_data {
	BmpImage pic;
	int nw;
	int nh;
	float ang;
	BmpReader* bmpReader;
};

struct shear_data_2 {
	BmpImage pic;
	int nw;
	int nh;
	float ang;
	BmpReader* bmpReader;
};

struct final_data {
	BmpImage pic;
	int nw;
	int nh;
	float ang1, ang2;
	float rot;
	BmpReader* bmpReader;
};


int main(){

	struct timeval  tv1, tv2;

	cout << "Without using pthread" << endl;

	// timing function 1
	gettimeofday(&tv1, NULL);

	BmpReader* bmpReader = new BmpReader();
	BmpImage input_pic = bmpReader->ReadBMP(INPUT_IMAGE);
	int newWidth = (int)(input_pic.width * BILINEAR_RATIO);
	int newHeight = (int)(input_pic.height * BILINEAR_RATIO);

	//all image processing
	//workone
	BmpImage bilinear_pic = bilinear(input_pic, newWidth, newHeight);

	//worktwo
	BmpImage bilinear_pic_2 = bilinear(input_pic, newWidth, newHeight);
	BmpImage rotation_pic = rotation(bilinear_pic_2, newWidth, newHeight,45);

	//workthree
	BmpImage bilinear_pic_3 = bilinear(input_pic, newWidth, newHeight);
	BmpImage shearVert_pic = shear_vertical(bilinear_pic_3, newWidth, newHeight,0.3);

	//workfour
	BmpImage bilinear_pic_4 = bilinear(input_pic, newWidth, newHeight);
	BmpImage shearHori_pic = shear_horizontal(bilinear_pic_4, newWidth, newHeight,0.6);

	//workfive
	BmpImage bilinear_pic_5 = bilinear(input_pic, newWidth, newHeight);
	BmpImage shearVert_pic_2 = shear_vertical(bilinear_pic_5, newWidth, newHeight,0.45);
	BmpImage tmp_pic = rotation(shearVert_pic_2, newWidth, newHeight,60);
	BmpImage mix_pic = shear_horizontal(tmp_pic, newWidth, newHeight,0.45);

	//Write bmp files
	bmpReader->WriteBMP(OUT_IMAGE[0], bilinear_pic);
	bmpReader->WriteBMP(OUT_IMAGE[1], rotation_pic);
	bmpReader->WriteBMP(OUT_IMAGE[2], shearVert_pic);
	bmpReader->WriteBMP(OUT_IMAGE[3], shearHori_pic);
	bmpReader->WriteBMP(OUT_IMAGE[4], mix_pic);

	//free memory
	free(bilinear_pic.data);
	free(rotation_pic.data);
	free(shearVert_pic.data);
	free(shearHori_pic.data);
	free(tmp_pic.data);
	free(mix_pic.data);

	// timing function 2
	gettimeofday(&tv2, NULL);
	time_elapse_first=(double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	// TODO: modify (ID) to your student ID
	printf("(0610890)%f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));

	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
    void *status;

	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	cout << "Using pthread" << endl;

	// timing function 1
	gettimeofday(&tv1, NULL);

	//bilinear_pic = bilinear(input_pic, newWidth, newHeight);
	bilinear_data bilinear_temp;
	bilinear_temp.pic=input_pic;
	bilinear_temp.nw=newWidth;
	bilinear_temp.nh=newHeight;
	bilinear_temp.bmpReader=bmpReader;

	pthread_create(&threads[0], &attr, work_one, (void *)&bilinear_temp);

	//rotation_pic = rotation(bilinear_pic, newWidth, newHeight,45);
	rotation_data rotation_temp;
	rotation_temp.pic=input_pic;
	rotation_temp.nw=newWidth;
	rotation_temp.nh=newHeight;
	rotation_temp.bmpReader=bmpReader;
	rotation_temp.rot=45;
	pthread_create(&threads[1], NULL, work_two, (void *)&rotation_temp);

	//shearVert_pic = shear_vertical(bilinear_pic, newWidth, newHeight,0.3);
	shear_data shear_temp;
	shear_temp.pic=input_pic;
	shear_temp.nw=newWidth;
	shear_temp.nh=newHeight;
	shear_temp.ang=0.3;
	shear_temp.bmpReader=bmpReader;
	pthread_create(&threads[2], NULL, work_three, (void *)&shear_temp);

	//shearHori_pic = shear_horizontal(bilinear_pic, newWidth, newHeight,0.6);
	shear_data_2 shear_temp_2;
	shear_temp_2.pic=input_pic;
	shear_temp_2.nw=newWidth;
	shear_temp_2.nh=newHeight;
	shear_temp_2.ang=0.6;
	shear_temp_2.bmpReader=bmpReader;
	pthread_create(&threads[3], NULL, work_four, (void *)&shear_temp_2);


	//shearVert_pic_2 = shear_vertical(bilinear_pic, newWidth, newHeight,0.45);
	//tmp_pic = rotation(shearVert_pic_2, newWidth, newHeight,60);
	//mix_pic = shear_horizontal(tmp_pic, newWidth, newHeight,0.45);
	final_data final_temp;
	final_temp.pic=input_pic;
	final_temp.nw=newWidth;
	final_temp.nh=newHeight;
	final_temp.ang1=0.45;
	final_temp.ang2=0.45;
	final_temp.rot=60;
	final_temp.bmpReader=bmpReader;
	pthread_create(&threads[4], NULL, work_five, (void *)&final_temp);


	//wait for pthread to terminate
	pthread_attr_destroy(&attr);


    for(int i = 0; i < NUM_THREADS; i++ ){
    	rc = pthread_join(threads[i], &status);
    	if (rc) {
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
    	}
    	//cout << "Main: completed thread id :" << i ;

    }

	//cout << "Main: program exiting." << endl;

	gettimeofday(&tv2, NULL);

	time_elapse_second=(double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);

	printf("(0610890)%f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));

	cout << "speed up rate: " << time_elapse_first/time_elapse_second << endl;

	pthread_exit(NULL);

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
				// TODO: assign a new value to this pixel
				// pixel =
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

BmpImage rotation(BmpImage pic,int nw,int nh, int rot)
{
	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	int w_offset = (int)(nw / 2);
	int h_offset = (int)(nh / 2);

	for(int i = 0 ; i < nw; i++){
		for(int j = 0; j < nh; j++){
			for(int k = 0 ; k < 3 ; k++){

				double angle = (double)rot * M_PI / 180.0;
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


BmpImage shear_vertical(BmpImage pic, int nw, int nh, float ang)
{

	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;
				int h_offset = (int)(nw * ang /2);
				int trans_w = i;
				int trans_h = (int)(j + i * ang) - h_offset;

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

BmpImage shear_horizontal(BmpImage pic, int nw, int nh, float ang)
{


	unsigned char* data	= (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for(int i = 0 ; i < nw ; i++){
		for(int j = 0 ; j < nh ; j++){
			//k is RGB color, R = 2, G = 1, B = 0
			for(int k = 0;k < 3;k++){
				int pixel = 0;

				// TODO: refer to shear_vertical, try to write shear_horizontal on your own
				int w_offset = (int)(nh * ang /2);
				int trans_w = (int)(i + j * ang) - w_offset;
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

void *work_one(void *threadid){
	struct bilinear_data *my_data;

	my_data = (bilinear_data *)threadid;

	BmpImage pic = my_data->pic;
	int newWidth = my_data->nw;
	int newHeight = my_data->nh;

	BmpImage bilinear_pic = bilinear(pic, newWidth, newHeight);

	my_data->bmpReader->WriteBMP(OUT_IMAGE[0], bilinear_pic);

	pthread_exit(NULL);

}

void *work_two(void *threadid){

	struct rotation_data *my_data;

	my_data = (rotation_data *)threadid;

	BmpImage pic = my_data->pic;
	int nw = my_data->nw;
	int nh = my_data->nh;
	int rot = my_data->rot;

	BmpImage bilinear_pic = bilinear(pic, nw, nh);
	BmpImage rotation_pic = rotation(bilinear_pic, nw, nh,rot);

	my_data->bmpReader->WriteBMP(OUT_IMAGE[1], rotation_pic);

	pthread_exit(NULL);
}

void *work_three(void *threadid){
	struct shear_data *my_data;

	my_data = (shear_data *)threadid;

	BmpImage pic = my_data->pic;
	int nw = my_data->nw;
	int nh = my_data->nh;
	float ang = my_data->ang;

	BmpImage bilinear_pic = bilinear(pic, nw, nh);
	BmpImage shearVert_pic = shear_vertical(bilinear_pic, nw, nh,ang);

	my_data->bmpReader->WriteBMP(OUT_IMAGE[2], shearVert_pic);

	pthread_exit(NULL);

}

void *work_four(void *threadid){
	struct shear_data_2 *my_data;

	my_data = (shear_data_2 *)threadid;

	BmpImage pic = my_data->pic;
	int nw = my_data->nw;
	int nh = my_data->nh;
	float ang = my_data->ang;

	BmpImage bilinear_pic = bilinear(pic, nw, nh);
	BmpImage shearHori_pic = shear_horizontal(bilinear_pic, nw, nh,ang);

	my_data->bmpReader->WriteBMP(OUT_IMAGE[3], shearHori_pic);

	pthread_exit(NULL);


}

void *work_five(void *threadid){
	struct final_data *my_data;

	my_data = (final_data *)threadid;

	BmpImage pic = my_data->pic;
	int nw = my_data->nw;
	int nh = my_data->nh;
	int rot = my_data->rot;
	float ang1 = my_data->ang1;
	float ang2 = my_data->ang2;

	BmpImage bilinear_pic = bilinear(pic, nw, nh);
	BmpImage shearVert_pic = shear_vertical(bilinear_pic, nw, nh,ang1);
	BmpImage rotation_pic = rotation(shearVert_pic, nw, nh,rot);
	BmpImage mix_pic = shear_horizontal(rotation_pic, nw, nh,ang2);

	my_data->bmpReader->WriteBMP(OUT_IMAGE[4], mix_pic);

	pthread_exit(NULL);

}

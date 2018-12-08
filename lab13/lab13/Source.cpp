#define CURL_STATICLIB 
#include "curl/curl.h" 
#include <stdio.h> 
#include <fstream>
#include <string> 
#include <iostream> 
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\objdetect.hpp>
#include <openssl/sha.h>
#include <openssl/conf.h> // �������, ��������� � ��������� ��������� OpenSSL
#include <openssl/conf.h>
#include <openssl/evp.h> // ���� ���������������� ������� https://wiki.openssl.org/index.php/EVP
#include <openssl/err.h> // ���� ���������� ������ OpenSSL � �� �����������
#include <openssl/aes.h>
#ifdef _WIN64 
#pragma comment (lib, "Normaliz.lib") 
#pragma comment (lib, "Ws2_32.lib") 
#pragma comment (lib, "Wldap32.lib") 
#pragma comment (lib, "Crypt32.lib") 
#pragma comment (lib, "advapi32.lib") 
#elif _WIN32 
#pragma comment (lib, "Normaliz.lib") 
#pragma comment (lib, "Ws2_32.lib") 
#pragma comment (lib, "Wldap32.lib") 
#pragma comment (lib, "Crypt32.lib") 
#pragma comment (lib, "advapi32.lib") 
#endif 
using namespace std;
size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
	FILE* stream = (FILE*)userdata;
	if (!stream)
	{
		printf("!!! No stream\n");
		return 0;
	}

	size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
	return written;
}

bool download_jpeg(char* url)
{
	FILE* fp = fopen("out.jpg", "wb");
	if (!fp)
	{
		printf("!!! Failed to create file on the disk\n");
		return false;
	}
	CURL* curlCtx = curl_easy_init();
	curl_easy_setopt(curlCtx, CURLOPT_URL, url);
	curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
	curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

	CURLcode rc = curl_easy_perform(curlCtx);
	if (rc)
	{
		printf("!!! Failed to download: %s\n", url);
		return false;
	}

	long res_code = 0;
	curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
	if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
	{
		printf("!!! Response code: %d\n", res_code);
		return false;
	}

	curl_easy_cleanup(curlCtx);

	fclose(fp);

	return true;
}

int main(int argc, char** argv)
{
	char * str = (char *)"http://pre01.deviantart.net/b620/th/pre/f/2015/144/8/1/sad_pikachu_by_bekkistevenson-d8ujru5.png";
//https://get.wallhere.com/photo/illustration-anime-cartoon-Ano-Hi-Mita-Hana-no-Namae-wo-Bokutachi-wa-Mada-Shiranai-mangaka-43667.jpg - ����� -���� ����
// https://w-dog.ru/wallpapers/6/2/326939737322301.jpg - �������- ���� ����
//http://www.kartinki.me/download.php?img=201208/1440x900/kartinki.me-5962.jpg - ������ ���� ����
//http://images6.fanpop.com/image/photos/32400000/Anime-anime-32439800-2560-1440.jpg - �����  ����
//http://pre01.deviantart.net/b620/th/pre/f/2015/144/8/1/sad_pikachu_by_bekkistevenson-d8ujru5.png - ������ ��� ���
//http://www.2fons.ru/pic/201501/1920x1080/2fons.ru-71678.jpg - ������� ��� ���
	download_jpeg(str);
	cv::CascadeClassifier face_cascade;
	cv::Mat image = cv::imread("out.jpg");
	cv::Mat groupFaces = cv::Mat(image.rows, image.cols, CV_8UC4);
	cv::cvtColor(image, groupFaces, cv::COLOR_BGR2BGRA);
	//cv::winrt_initContainer(cvContainer);
	cv::imshow(std::string("Window 1"), groupFaces);

	if (!face_cascade.load("haarcascade_frontalface_alt.xml")) {
		std::cout << "Couldn't load face detector" << std::endl;
	}
	int coll_face;
	if (!groupFaces.empty()) {
		std::vector<cv::Rect> facesColl;
		cv::Mat frame_gray;

		cvtColor(groupFaces, frame_gray, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(frame_gray, frame_gray);

		// Detect faces
		face_cascade.detectMultiScale(frame_gray,
			facesColl,
			1.1, 2,
			0 | cv::CASCADE_SCALE_IMAGE,
			cv::Size(1, 1));
		coll_face=facesColl.size();
		for (unsigned int i = 0; i < facesColl.size(); i++)
		{
			auto face = facesColl[i];
			cv::rectangle(groupFaces, face, cv::Scalar(0, 255, 255), 5);
		}

		cv::imshow(std::string("Window 2"), groupFaces);
	}
	else {
		std::cout << "Initialize image before processing" << std::endl;
	}
	std::ofstream fout("dont_open.txt");
	if (coll_face != 0)//�oll_face -���������� �������� ���
	{	
		fout << "face  detect - " << coll_face << endl;//����� ���-�� ���
	}
	else
	{
		fout << "face non detect" << endl;
	}
	FILE * fout_sha1;//�������� ���� ��� �������
	fout_sha1 = fopen("out_sha1.txt", "w");
	//unsigned char ibuf[] = (unsigned char) str;
	unsigned char obuf[20];//�������� �������� ��� �������
	SHA1((const unsigned char *)str, strlen((const char *)str), obuf);
	for (int i = 0; i < 20; i++) {
		fprintf(fout_sha1, "%02x ", obuf[i]);
	}
	struct name_of_my_struct // ������ ������
	{
		name_of_my_struct()
		{

		}

		int a;
		double b;
		int fnc1()
		{
			return a;
		}
	};
	if (coll_face !=0 )
	{
	unsigned char *plaintext =
		(unsigned char *)str;// �������� �����
	int plaintext_len = strlen((char *)plaintext); // ����� ������
	unsigned char *key = (unsigned char *)"0123456789"; // ������ (����)
	unsigned char *iv = (unsigned char *)"0123456789012345"; // ���������������� ������, �����������
	unsigned char cryptedtext[256]; // ������������� ���������
	unsigned char decryptedtext[256]; // �������������� ���������
	


		// 1. �������� ��������� �� �������������� ���������
		// ��������� - ��� ������ � C++, ������ � ������, �������� ����������
		EVP_CIPHER_CTX *ctx; // structure

		// 2. ��� ��������� �������� ������ ��������� �������� (�����, ����, ������ ������������� � �.�.)
		ctx = EVP_CIPHER_CTX_new(); // �������� ��������� � ����������� ������

		// 3. ��������� EVP_CIPHER_CTX ����������� �����������
		EVP_EncryptInit_ex(ctx, // ������ �� ������/���������, ���� ��������� ���������
			EVP_aes_256_cbc(), // ������ �� ��������� ���� AES 256 (������� � ����������)
			NULL,
			key, // ����/������/������
			iv); // ����������� (��������� ��������� ������)

		// 4. ��� ������� ���������� - ������� EVP_EncryptUpdate
		int len;
		EVP_EncryptUpdate(ctx, // ������ � �����������
			cryptedtext, // ������� ��������: ������, ���� �������� ������������� ������
			&len, // ������� ��������: ����� ����������� �����
			plaintext, // ������� ��������: ��� ���������
			plaintext_len); // ������� �������� : ����� ������� ������
		int cryptedtext_len = len;

		// 5. ����������� �������� ����������
		// ����������, ���� ��������� ���� �������� ������� �� ���������
		EVP_EncryptFinal_ex(ctx, cryptedtext + len, &len);
		cryptedtext_len += len;

		// 6. �������� ���������
		EVP_CIPHER_CTX_free(ctx);
		// ����� ������������� ������
		for (int i = 0; i < cryptedtext_len; i++)
		{
			fout << hex << cryptedtext[i];
			if ((i + 1) % 32 == 0) cout << endl;
		}
		fout << endl;

		// �����������
		ofstream fout_c("Decrypt.txt");
		// 1.
		ctx = EVP_CIPHER_CTX_new(); // �������� ��������� � ����������� ������

		// 2.
		EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv); // ������������� ������� AES, ������ � ��������

		// 3.
		EVP_DecryptUpdate(ctx, decryptedtext, &len, cryptedtext, cryptedtext_len);  // ����������, ����������

		// 4.
		int dectypted_len = len;
		EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len);

		// 5.
		dectypted_len += len;
		EVP_CIPHER_CTX_free(ctx);
		decryptedtext[dectypted_len] = '\0';
		fout_c << decryptedtext << endl;
	}
	else
	{
		fout << str;
	}

	cv::waitKey();

	return 0;
}
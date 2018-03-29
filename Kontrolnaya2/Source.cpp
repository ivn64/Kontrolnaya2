#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <conio.h>

void FileOpen(BITMAPFILEHEADER &bfh, BITMAPINFOHEADER &bih, RGBQUAD *&originalRgb)
{
	FILE *originalFile;
	originalFile = fopen("1.bmp", "r+b");
	fread(&bfh, sizeof(bfh), 1, originalFile);						//файловый заголовок в структуру BITMAPFILEHEADER
	fread(&bih, sizeof(bih), 1, originalFile);						//заголовок изображени€ в структуру BITMAPINFOHEADER
	originalRgb = (RGBQUAD*)malloc(bih.biHeight * bih.biWidth * sizeof(RGBQUAD));
	fread(originalRgb, bih.biHeight * bih.biWidth * sizeof(RGBQUAD), 1, originalFile);
	fclose(originalFile);
}

void FileWrite(BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih, RGBQUAD *changeRgb)
{
	FILE *newFile;
	newFile = fopen("2.bmp", "w+b");
	fwrite(&bfh, sizeof(bfh), 1, newFile);
	fwrite(&bih, sizeof(bih), 1, newFile);
	fwrite(changeRgb, bih.biHeight * bih.biWidth * sizeof(RGBQUAD), 1, newFile);
	free(changeRgb);
	fclose(newFile);
}

void Contrast()
{
	int step0 = 0;
	printf("на сколько изменить контраст?\n");
	scanf("%d", &step0);
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	RGBQUAD *originalRgb;

	FileOpen(bfh, bih, originalRgb);

	int sumRGB = 0;
	double step = 1 + step0*0.01; //коэффициент

	for (int i = 0; i < bih.biHeight; i++)
	{
		for (int j = 0; j < bih.biWidth; j++)
		{
			sumRGB += 0.114*originalRgb[i * bih.biWidth + j].rgbBlue + 0.587*originalRgb[i * bih.biWidth + j].rgbGreen + 0.299*originalRgb[i * bih.biWidth + j].rgbRed;
		}
	}
	int y = sumRGB / (bih.biHeight * bih.biWidth); //средн€€ €ркость
	for (int i = 0; i < bih.biHeight; i++)
	{
		for (int j = 0; j < bih.biWidth; j++)
		{
			int temp;
			temp = y + step*(originalRgb[i * bih.biWidth + j].rgbBlue - y);
			if (temp > 255)
				originalRgb[i * bih.biWidth + j].rgbBlue = 255;
			else
				if (temp < 0)
					originalRgb[i * bih.biWidth + j].rgbBlue = 0;
				else
					originalRgb[i * bih.biWidth + j].rgbBlue = temp;

			temp = y + step*(originalRgb[i * bih.biWidth + j].rgbGreen - y);
			if (temp > 255)
				originalRgb[i * bih.biWidth + j].rgbGreen = 255;
			else
				if (temp < 0)
					originalRgb[i * bih.biWidth + j].rgbGreen = 0;
				else
					originalRgb[i * bih.biWidth + j].rgbGreen = temp;

			temp = y + step*(originalRgb[i * bih.biWidth + j].rgbRed - y);
			if (temp > 255)
				originalRgb[i * bih.biWidth + j].rgbRed = 255;
			else
				if (temp < 0)
					originalRgb[i * bih.biWidth + j].rgbRed = 0;
				else
					originalRgb[i * bih.biWidth + j].rgbRed = temp;
		}
	}
	FileWrite(bfh, bih, originalRgb);
}

void Brightness()
{
	int step = 0;
	printf("на сколько изменить €ркость?\n");
	scanf("%d", &step);
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	RGBQUAD *originalRgb;

	FileOpen(bfh, bih, originalRgb);
	
	for (int i = 0; i < bih.biHeight; i ++)
	{
		for (int j = 0; j < bih.biWidth; j ++)
		{
			int temp;
			temp = originalRgb[i * bih.biWidth + j].rgbBlue + step;
			if (temp > 255)
				originalRgb[i * bih.biWidth + j].rgbBlue = 255;
			else
				if (temp < 0)
					originalRgb[i * bih.biWidth + j].rgbBlue = 0;
				else
					originalRgb[i * bih.biWidth + j].rgbBlue = temp;

			temp = originalRgb[i * bih.biWidth + j].rgbGreen + step;
			if (temp > 255)
				originalRgb[i * bih.biWidth + j].rgbGreen = 255;
			else
				if (temp < 0)
					originalRgb[i * bih.biWidth + j].rgbGreen = 0;
				else
					originalRgb[i * bih.biWidth + j].rgbGreen = temp;

			temp = originalRgb[i * bih.biWidth + j].rgbRed + step;
			if (temp > 255)
				originalRgb[i * bih.biWidth + j].rgbRed = 255;
			else
				if (temp < 0)
					originalRgb[i * bih.biWidth + j].rgbRed = 0;
				else
					originalRgb[i * bih.biWidth + j].rgbRed = temp;
		}
	}
	FileWrite(bfh, bih, originalRgb);
}

void ResizeDown()
{
	int step = 0;
	printf("во сколько раз уменьшить изображение?\n");
	scanf("%d", &step);
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	RGBQUAD *originalRgb;

	FileOpen(bfh, bih, originalRgb);

	int newHeigh;
	int newWidth;

	if (bih.biHeight%step == 0)
		newHeigh = bih.biHeight / step;
	else
		newHeigh = bih.biHeight / step + 1;
	if (bih.biWidth%step == 0)
		newWidth = bih.biWidth / step;
	else
		newWidth = bih.biWidth / step + 1;

	RGBQUAD *changeRgb = (RGBQUAD*)malloc(newHeigh * newWidth * sizeof(RGBQUAD));

	int k = 0;

	for (int i = 0; i < bih.biHeight; i += step)
	{
		for (int j = 0; j < bih.biWidth; j += step)
		{
			changeRgb[k].rgbBlue = originalRgb[i * bih.biWidth + j].rgbBlue;
			changeRgb[k].rgbGreen = originalRgb[i * bih.biWidth + j].rgbGreen;
			changeRgb[k].rgbRed = originalRgb[i * bih.biWidth + j].rgbRed;
			k++;
		}
	}

	free(originalRgb);

	bih.biHeight = newHeigh;
	bih.biWidth = newWidth;
	bih.biSizeImage = newHeigh * newWidth;
	bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + newHeigh * newWidth * sizeof(RGBQUAD);

	FileWrite(bfh, bih, changeRgb);
}

void Mask()
{
	int step = 0;
	printf("на сколько изменить маску?\n");
	scanf("%d", &step);

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	RGBQUAD *originalRgb;

	FileOpen(bfh, bih, originalRgb);

	
	int answer = -1;
	do
	{
		printf("\n1 - син€€\n2 - зелЄна€\n3 - красна€\n0 - выход\n\n");
		printf("¬ведите значение: ");
		scanf("%d", &answer);
		switch (answer)
		{
		case 1:
			for (int i = 0; i < bih.biHeight; i++)
			{
				for (int j = 0; j < bih.biWidth; j++)
				{
					int temp;
					temp = originalRgb[i * bih.biWidth + j].rgbBlue + step;
					if (temp > 255)
						originalRgb[i * bih.biWidth + j].rgbBlue = 255;
					else
						if (temp < 0)
							originalRgb[i * bih.biWidth + j].rgbBlue = 0;
						else
							originalRgb[i * bih.biWidth + j].rgbBlue = temp;
				}
			}
			break;
		case 2:
			for (int i = 0; i < bih.biHeight; i++)
			{
				for (int j = 0; j < bih.biWidth; j++)
				{
					int temp;
					temp = originalRgb[i * bih.biWidth + j].rgbGreen + step;
					if (temp > 255)
						originalRgb[i * bih.biWidth + j].rgbGreen = 255;
					else
						if (temp < 0)
							originalRgb[i * bih.biWidth + j].rgbGreen = 0;
						else
							originalRgb[i * bih.biWidth + j].rgbGreen = temp;
				}
			}
		case 3:
			for (int i = 0; i < bih.biHeight; i++)
			{
				for (int j = 0; j < bih.biWidth; j++)
				{
					int temp;
					temp = originalRgb[i * bih.biWidth + j].rgbRed + step;
					if (temp > 255)
						originalRgb[i * bih.biWidth + j].rgbRed = 255;
					else
						if (temp < 0)
							originalRgb[i * bih.biWidth + j].rgbRed = 0;
						else
							originalRgb[i * bih.biWidth + j].rgbRed = temp;
				}
			}
			break;
		case 0:
			break;
		default:
			printf("—делайте правильный выбор!");
			break;
		}
	} while (answer < 0 && answer > 4);
	FileWrite(bfh, bih, originalRgb);
}



void main()
{
	setlocale(LC_ALL, "rus");

	int answer = -1;
	do
	{
		printf("ћеню:\n1 - уменьшить изображение\n2 - €ркость\n3 - контрастность\n4 - маска цвета\n0 - выход\n\n");
		printf("¬ведите значение: ");
		scanf("%d", &answer);
		switch (answer)
		{
		case 1:
			ResizeDown();
			break;
		case 2:
			Brightness();
			break;
		case 3:
			Contrast();
			break;
		case 4:
			Mask();
			break;
		case 0:
			break;
		default:
			printf("—делайте правильный выбор!");
			break;
		}
	} while (answer != 0);
}
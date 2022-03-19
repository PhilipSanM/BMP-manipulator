/*      =======================================================================
        ===================INSTITUTO POLITECNICO NACIONAL======================
        =======================================================================
                            =============================
                            =ESCUELA SUPERIOR DE COMPUTO=
                            =============================
>>Name: Sánchez Martánez Felipe
>>Gruop: 2BM1
>>Carrer: Artificial Intelligence engineer
>>date of last modification: 17/12/21
                   

*/

#include "librery.h"

/**	errores
 *	1 - Archivo no existe
 *	2 - Archivo no cumple requisitos
 *	3 - no existe la ruta
 *	4 - no hay extensión BMP
 *	5 - no es formato BMP
 */

int loadImage(Image* image, char* path){
	FILE *fileImage = NULL;
	if (strlen(path) == 0)				
		return 3;
	else if (strchr(path, '.') == NULL)		
		return 4;
	
	fileImage = fopen(path, "rb");
	if (!fileImage)	
		return 1;

	fread(&image->signatureFile, sizeof(uint16_t), 1, fileImage);
	if (!isBMP(image->signatureFile))			// is bmp file?
		return 5;

	fread(&image->header, sizeof(IHDR), 1, fileImage);
	fread(&image->data,   sizeof(IDAT), 1, fileImage);

	if (image->data.height != VALIDSIZE || image->data.width!= VALIDSIZE)
		return 2;


	int longRGBDQuad=(image->header.offset)-sizeof(IDAT)-sizeof(IDAT)-2;
	//printf("longRGBDQuad=%d\n",longRGBDQuad);
	//se lee el cuadro de enmedio
	image->rgbquad= malloc(longRGBDQuad);
	if(image->rgbquad==NULL){
		puts("No se pudo reservar memoria");
		fclose(fileImage);
		exit(1);
	}
	fread(image->rgbquad, longRGBDQuad, 1, fileImage);
	for(int k=0; k>longRGBDQuad;k++){
		printf("valor rgbqu[%d]=%d\n",k,image->rgbquad[k]);
	}
	fseek(fileImage, image->header.offset, SEEK_SET);
	image->pxs = malloc(image->data.imgSize);
	if(image->pxs==NULL){
		puts("No se pudo reservar memoria para los datos");
		free(image->rgbquad);
		fclose(fileImage);
		exit(1);
	}
	fread(image->pxs, image->data.imgSize, 1, fileImage);
	fclose(fileImage);

	return 0;
}

int saveImage(Image* image){
	FILE *fileImage = NULL;
	
	strcat(image->name, "2.bmp");
	fileImage = fopen(image->name, "wb+");
	if (!fileImage)
		return 1;

	fwrite(&image->signatureFile, sizeof(uint16_t), 1, fileImage);
	fwrite(&image->header, 1, sizeof(IHDR), fileImage);
	fwrite(&image->data,   1, sizeof(IDAT), fileImage);
	fwrite(&image->rgbquad,   1, sizeof(image->rgbquad), fileImage);
	fwrite(image->pxs, image->data.imgSize, 1, fileImage);

	fclose(fileImage);
	printf("%s created \n", image->name);
	return 0;
}

void initImage(Image* copyImg, Image* original){
	copyImg->signatureFile = BMP;

	copyImg->header.AdicionalCarecteristics = original->header.AdicionalCarecteristics;
	copyImg->header.copyReserved = original->header.copyReserved;
	copyImg->header.offset = original->header.offset;
	copyImg->header.size	= original->header.size;

	copyImg->data.bpp = original->data.bpp;
	copyImg->data.channels = original->data.channels;
	copyImg->data.colorsRange = original->data.colorsRange;
	copyImg->data.compression = original->data.compression;
	copyImg->data.height = original->data.height;
	copyImg->data.ihdrSize = original->data.ihdrSize;
	copyImg->data.imgSize = original->data.imgSize;
	copyImg->data.imxtcolors = original->data.imxtcolors;
	copyImg->data.resX = original->data.resX;
	copyImg->data.resY = original->data.resY;
	copyImg->data.width = original->data.width;

}

void copyImagen(Image* imageResult, Image* originalImage){
	initImage(imageResult, originalImage);
	imageResult->pxs = malloc(originalImage->data.imgSize*sizeof(char));
	imageResult->rgbquad=originalImage->rgbquad;
	for (int i = 0; i < originalImage->data.imgSize; i++)
		imageResult->pxs[i] = originalImage->pxs[i];
}

unsigned char *sum(Image* imageFirst, Image* imageSecond, Image* imagesum){
    initImage(imagesum, imageSecond);
    imagesum->pxs = malloc(imageSecond->data.imgSize*sizeof(char));
	imagesum->rgbquad=imageSecond->rgbquad;
    int ejeX, ejeY, positionX=0, positionY=0;

    for (int i = 0; i < 512; i++)
	imagesum->pxs[i] = imageSecond->pxs[i];

    return imagesum->pxs;
}

void sumA(Image* imageFirst, Image* imageSecond, Image* imagesum){
    // initImage(imagesum, imageSecond);
    // imagesum->pxs = malloc(imageSecond->data.imgSize*sizeof(char));
	// imagesum->rgbquad=imageSecond->rgbquad;

    // for (int i = 0; i < 256; i++)
	//     imagesum->pxs[i] = imageSecond->pxs[i];

	initImage(imagesum, imageFirst);
	imagesum->pxs = malloc(imageFirst->data.imgSize*sizeof(char));
	imagesum->rgbquad=imageFirst->rgbquad;
	for (int i = 0; i < imageFirst->data.imgSize; i++)
		imagesum->pxs[i] = imageFirst->pxs[i]+imageSecond->pxs[i];


}

void main(){
    //LOAD OF IMAGES
    puts("START");

	Image Lena1;
	Lena1.name=malloc(9);
	if(Lena1.name==NULL){
		puts("The memory is finished");
		exit(1);
	}
	strcpy(Lena1.name,"lena.bmp");
    loadImage(&Lena1,Lena1.name);
    puts("Lena1 is loaded");

    Image Lena2;
	Lena2.name=malloc(9);
	if(Lena2.name==NULL){
		puts("The memory is finished");
		exit(1);
	}
	strcpy(Lena2.name,"lena_(2).bmp");
    loadImage(&Lena2,Lena2.name);
    puts("Lena2 is loaded");

    Image Lena3;
	Lena3.name=malloc(9);
	if(Lena3.name==NULL){
		puts("The memory is finished");
		exit(1);
	}
	strcpy(Lena3.name,"lena_(3).bmp");
    loadImage(&Lena3,Lena3.name);
    puts("Lena3 is loaded");

    puts("Start the sum of two lena");
    Image sum1;
    sum1.name=malloc(30);
	if(sum1.name==NULL){
		puts("The memory is finished");
		free(sum1.name); 
		exit(1);
	}
	strcpy(sum1.name,"sum of two Lena");

    sumA(&Lena1, &Lena2, &sum1);

    saveImage(&sum1);

    puts("Start the sum of three lena");
    Image sum2;
    sum2.name=malloc(30);
	if(sum2.name==NULL){
		puts("The memory is finished");
		free(sum2.name);  
		exit(1);
	}
	strcpy(sum2.name,"Sum of three Lena");

    sumA(&sum1, &Lena1, &sum2);
    saveImage(&sum2);
    free(Lena1.name);
    free(Lena2.name);
    free(Lena3.name);
    puts("END");

}

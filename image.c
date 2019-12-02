#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"

Image *image_init(int width, int height) {

  Image *image;

  /* U slucaju greske prekida se program. */
  assert(width >= 0 && height >= 0);

  /* Alocira se prostor za smestanje clanova strukture. */
  image = (Image *) malloc(sizeof(Image));
  assert(image != NULL);

  /* Inicijlizuju se clanovi strukture. */
  image->width = width;
  image->height = height;
  if (width == 0 || height == 0)
    image->pixels = NULL;
  else {
    image->pixels = (char *)malloc(3 * width * height * sizeof(char));
    assert(image->pixels != NULL);
  }

  /* Vraca se pokazivac na inicijlizovanu strukturu. */
  return image;
}

void image_done(Image *image) {

  /*
   * Oslobadja se prostor za cuvanje podataka o pikselima
   * a potom se oslobadja i prostor u kojem su cuvani
   * podaci same strukture.
   */
  free(image->pixels);
  free(image);
}

void image_read(Image *image, const char *filename) {

  FILE *file;
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  unsigned int i;
  unsigned char r, g, b, a;

  /* Brise se prethodni sadrzaj strukture Image. */
  free(image->pixels);
  image->pixels = NULL;

  /* Otvara se fajl koji sadrzi sliku u binarnom rezimu. */
  assert((file = fopen(filename, "rb")) != NULL);

  /* Ocitavaju se podaci prvog zaglavlja. */
  fread(&bfh.type, 2, 1, file);
  fread(&bfh.size, 4, 1, file);
  fread(&bfh.reserved1, 2, 1, file);
  fread(&bfh.reserved2, 2, 1, file);
  fread(&bfh.offsetbits, 4, 1, file);

  /* Ocitavaju se podaci drugog zaglavlja. */
  fread(&bih.size, 4, 1, file);
  fread(&bih.width, 4, 1, file);
  fread(&bih.height, 4, 1, file);
  fread(&bih.planes, 2, 1, file);
  fread(&bih.bitcount, 2, 1, file);
  fread(&bih.compression, 4, 1, file);
  fread(&bih.sizeimage, 4, 1, file);
  fread(&bih.xpelspermeter, 4, 1, file);
  fread(&bih.ypelspermeter, 4, 1, file);
  fread(&bih.colorsused, 4, 1, file);
  fread(&bih.colorsimportant, 4, 1, file);

  /*
   * Od podataka iz drugog zaglavlja koristimo samo informacije
   * o dimenzijama slike.
   */
  image->width = bih.width;
  image->height = bih.height;

  /*
   * U zavisnosti od toga koliko bitova informacija se cita po pikselu 
   * (da li samo R, G i B komponenta ili R, G, B i A), alociramo niz 
   * odgovarajuce duzine.
   */
  if (bih.bitcount == 24)
    image->pixels = (char *)malloc(3 * bih.width * bih.height * sizeof(char));
  else if (bih.bitcount == 32)
    image->pixels = (char *)malloc(4 * bih.width * bih.height * sizeof(char));
  else {
    fprintf(stderr, "image_read(): Podrzane su samo slike koje po pikselu cuvaju 24 ili 32 bita podataka.\n");
    exit(1);
  }
  assert(image->pixels != NULL);

  /* Ucitavaju se podaci o pikselima i smestaju u alocirani niz. */
  if (bih.bitcount == 24)
    /*
     * Ako se po pikselu cita 24 bita = 3 bajta informacija, pretpostavljamo 
     * da oni (ta 3 bajta) predstavljaju R, G i B komponentu boje (1 bajt po 
     * komponenti).
     */
    for (i = 0; i < bih.width * bih.height; i++) {
      /*
       * Ovo mozda izgleda cudno, to sto se komponente boje citaju u suprotnom redosledu, 
       * tj. prvo plava, pa zelena, pa crvena, ali tako pise u specifikaciji bmp formata.
       */
      fread(&b, sizeof(char), 1, file);
      fread(&g, sizeof(char), 1, file);
      fread(&r, sizeof(char), 1, file);

      image->pixels[3 * i] = r;
      image->pixels[3 * i + 1] = g;
      image->pixels[3 * i + 2] = b;
    }
  else if (bih.bitcount == 32)
    /*
     * Ako se po pikselu cita 32 bita = 4 bajta informacija, pretpostavljamo 
     * da oni (ta 4 bajta) predstavljaju R, G, B i A komponentu boje (1 bajt po 
     * komponenti).
     */
    for (i = 0; i < bih.width * bih.height; i++) {
      fread(&b, sizeof(char), 1, file);
      fread(&g, sizeof(char), 1, file);
      fread(&r, sizeof(char), 1, file);
      fread(&a, sizeof(char), 1, file);

      image->pixels[4 * i] = r;
      image->pixels[4 * i + 1] = g;
      image->pixels[4 * i + 2] = b;
      image->pixels[4 * i + 3] = a;
    }

  /* Zatvara se fajl. */
  fclose(file);
}

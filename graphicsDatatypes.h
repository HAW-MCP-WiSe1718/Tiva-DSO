#ifndef GRAPHICSDATATYPES_H_
#define GRAPHICSDATATYPES_H_

typedef struct{
    uint8_t reserved;

    uint8_t redValue;
    uint8_t greenValue;
    uint8_t blueValue;
}colorStruct;

typedef union{
    uint32_t colorValue;
    colorStruct colorSeperate;
}color;

typedef struct{
    uint16_t xPosition;
    color curserColor;
    uint8_t curserVisible;
}curserStruct;

/*position is defined in the top left pixel of the character*/
typedef struct{
    uint16_t xPosition;
    uint16_t yPosition;

    color characterColor;

    uint8_t characterPixel[5];

}character;

#endif /* GRAPHICSDATATYPES_H_ */

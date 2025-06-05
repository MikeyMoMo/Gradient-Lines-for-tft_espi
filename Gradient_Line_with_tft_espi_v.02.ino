// This ONLY draws orthogonal lines, NO diagonals!

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite fsSprite = TFT_eSprite(&tft);

//bool doShow = false;
float pctBlend;
uint16_t blendedColor;

/***************************************************************************/
void setup()
/***************************************************************************/
{
  Serial.begin(115200); delay(2000);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  int *a = 0;
  a = (int*)fsSprite.createSprite(tft.width(), tft.height());
  if (a == 0) {
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Sprite creation failed", tft.width() / 2, 30, 4);
    tft.drawString("Cannot continue.", tft.width() / 2, 90, 4);
    Serial.println("fsSprite creation failed.  Cannot continue.");
    while (1);
  }
  Serial.printf("createfsSprite returned: %p\r\n", a);

  fsSprite.fillSprite(TFT_WHITE);

  ledcAttach(TFT_BL, 5000, 8);  // PWM timer automatically assigned.
  ledcWrite(TFT_BL, 200);       // Turn the display on bigly for init messages.
}
/***************************************************************************/
void loop()
/***************************************************************************/
{

  //Vertical lines

  Serial.println("Vertical part 1");
  for (int x = 0; x < fsSprite.width() / 2; x++) {
    drawGradientLine (&fsSprite, x, fsSprite.height() - 1, x, 0,
                      0, TFT_RED, TFT_BLUE);  // top, bottom
  }
  fsSprite.pushSprite(0, 0);
  delay(1000);

  Serial.println("Vertical part 2");
  for (int x = tft.width() / 2; x < fsSprite.width(); x++) {
    drawGradientLine (&fsSprite, x, fsSprite.height() - 1, x, 0,
                      0, TFT_GREEN, TFT_WHITE);  // top, bottom
  }
  fsSprite.pushSprite(0, 0);

  delay(2000);
  fsSprite.fillSprite(TFT_WHITE);

  // Horizontal lines

  Serial.println("Horizontal part 1");
  for (int y = 0; y < fsSprite.height() / 2 - 1 ; y++)
    drawGradientLine (&fsSprite, 0, y, fsSprite.width(), y,
                      0, TFT_BLUE, TFT_RED);  // right, left

  fsSprite.pushSprite(0, 0);
  delay(1000);

  Serial.println("Horizontal part 2");
  for (int y = tft.height() / 2; y < tft.height() - 1; y++)
    drawGradientLine (&fsSprite, 0, y, fsSprite.width(), y,
                      0, TFT_WHITE, TFT_GREEN);  // right, left
  fsSprite.pushSprite(0, 0);
  delay(2000);
  fsSprite.fillSprite(TFT_BLACK);

  // Variable length

  Serial.println("Variable length, variable gradient.");
  for (int x = 0; x < fsSprite.height(); x++) {
    drawGradientLine (&fsSprite, x, x, x, fsSprite.height(),
                      0, TFT_BLUE, TFT_RED);  // bottom, top
  }
  fsSprite.drawString("Variable gradient", 100, 20, 4);
  fsSprite.pushSprite(0, 0);
  delay(5000);
  fsSprite.fillSprite(TFT_BLACK);

  Serial.println("Variable length, fixed gradient.");
  for (int x = 0; x < fsSprite.height(); x++) {
    drawGradientLine (&fsSprite, x, fsSprite.height(), x, x,
                      fsSprite.height(),  TFT_RED, TFT_BLUE);  // top, bottom
  }
  fsSprite.drawString("Fixed gradient", 100, 20, 4);

  fsSprite.pushSprite(0, 0);

  delay(5000);
  fsSprite.fillSprite(TFT_WHITE);
}
/***************************************************************************/
void drawGradientLine(TFT_eSprite *targetSprite,
                      int x1, int y1, int x2, int y2,
                      int blendLvls, uint16_t colorStart, uint16_t colorEnd)
/***************************************************************************/
{
  static int x, y, steps;

  // The 6th value (steps) changes the look of the gradient line.
  //  If it is 0, the gradient is completed along the length of the
  //  line no matter how long or short.  If there is a value here, its
  //  value is used for all lines. This gives a more regular look
  //  to the gradient lines.

  // steps is the length of the line in pixels.
  steps = abs(x2 - x1) > abs(y2 - y1) ? abs(x2 - x1) : abs(y2 - y1);
  // if blandLvls came in as 0, use the line pixel length for step count
  if (blendLvls == 0) blendLvls = steps;
  if (blendLvls < steps) {
    Serial.printf("BlendLvls (%i px) is less than line length "
                  "(%i px). Please correct and recompile.\r\n",
                  blendLvls, steps);
  }
  x = x1; y = y1;
  for (int i = 0; i <= steps; i++) {
    pctBlend = (float)i / (float)blendLvls;
    blendedColor = alphaBlend((uint8_t)(pctBlend * 255), colorStart, colorEnd  );
    targetSprite->drawPixel(x, y, blendedColor);

    if ((x1 - x2) == 0) y2 > y1 ? y++ : y--;  // Am I drawing horizontal or
    if ((y1 - y2) == 0) x2 > x1 ? x++ : x--;  // vertical. This decides.

  }
}
///***************************************************************************/
//void drawGradientLine(int x0, int y0, int x1, int y1, uint16_t colorStart,
//                      uint16_t colorEnd)
///***************************************************************************/
//{
//  int steps = abs(x1 - x0) > abs(y1 - y0) ? abs(x1 - x0) : abs(y1 - y0);
//  if (doShow) Serial.printf("Steps: %i\r\n", steps);
//  int x = x0, y = y0;
//  for (int i = 0; i <= steps; i++) {
//    pctBlend = (float)i / steps;
//  blendedColor = alphaBlend((uint8_t)(pctBlend * 255),
//                            colorEnd, colorStart);
//  if (doShow) Serial.printf("x %i, y %i, pctBlend %f\r\n", x, y,
//                             pctBlend);
//    fsSprite.drawPixel(x, y, blendedColor);
//    if ((x0 - x1) == 0) y1 > y0 ? y++ : y--;
//    if ((y0 - y1) == 0) x1 > x0 ? x++ : x--;
//  }
//}
/***************************************************************************/
uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
/***************************************************************************/
{
  uint32_t rxb = bgc & 0xF81F;
  rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
  uint32_t xgx = bgc & 0x07E0;
  xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
  return (rxb & 0xF81F) | (xgx & 0x07E0);
}

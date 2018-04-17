/**
 *	CoolBoardExample
 *
 *	This example shows typical use
 *	of the CoolBoard.
 *
 *	Save this example in another location
 *	in order to safely modify the configuration files
 *	in the data folder.
 *
 */

#include <CoolBoard.h>

CoolBoard coolBoard;

void setup() {
  Serial.begin(115200);
  coolBoard.config();
  coolBoard.begin();

  coolBoard.printConf();

  Serial.print("one log every ");
  Serial.print(coolBoard.getLogInterval());
  Serial.println(" s ");
}

void loop() {
  delay(100);
  Serial.print("one log");
  if (coolBoard.isConnected() == 0) {
    coolBoard.onLineMode();
  } else {
    coolBoard.offLineMode();
  }
}

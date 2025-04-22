#include "Arduino.h"
#include "Observer.h"
#include "Subject.h"
#include "U8g2lib.h"
#include "LCDMenuLib2.h"
#include "../common.h"
#include "menu.h"


class DisplayEngine : public Observer<EventArgs>,  public Subject<EventArgs>
{
  public:
    DisplayEngine();
	void notify(EventArgs args) override;
    void init();
    void work();
    void produceFatal(char *message, uint8_t code);


  private:
    void handleAudioEventForDisplay(EventArgs args);

};

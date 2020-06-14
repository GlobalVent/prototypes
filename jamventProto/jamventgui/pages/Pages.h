#ifndef PAGES_H
#define PAGES_H

//#include <QHash>
//#include <QtGlobal>

class Pages
{
public:
#if 0
    enum class Page_E
    {
        Powerup,
        Treatment,
        Count
    };
#else
  // Use enum instead of enum class as int supported by QHash where this is used.
  enum Page_E
  {
      PagePowerup,
      PageTreatment,
      PageCount
  };

#endif
};
#endif // PAGES_H

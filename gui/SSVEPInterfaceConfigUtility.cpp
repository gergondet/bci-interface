#include "ConfigUtil.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    ConfigUtil * conf = 0;

    if(argc > 2)
    {
        conf = new ConfigUtil(0, argv[1]);
    }
    else
    {
        conf = new ConfigUtil();
    }

    conf->show();

    int ret = app.exec();

    delete conf;

    return ret;
}

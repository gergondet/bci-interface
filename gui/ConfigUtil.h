#include "ui_configUtil.h"

class ConfigUtil : public QWidget, private Ui::configUtil
{
    Q_OBJECT

    public:
        ConfigUtil(QWidget * parent = 0, const char * configFile = "ssvpinterface-config");
    
    private slots:
        void on_exitButton_clicked();
        void on_applyButton_clicked();
        void on_addButton_clicked();
        void on_editColorButton_clicked();

    private:
        void AddRow(const char * frequency, const char * screenFreq,
                    const char * x, const char * y, const char * size);
        void SaveToConfig();
    
    private:
        const char * m_configFile;
        int m_r, m_g, m_b;
};


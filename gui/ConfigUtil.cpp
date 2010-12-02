#include "ConfigUtil.h"
#include <QtGui/QColorDialog>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

ConfigUtil::ConfigUtil(QWidget * parent, const char * configFile) : QWidget(parent), m_configFile(configFile), m_r(0), m_g(0), m_b(0)
{
    setupUi(this);
    std::ifstream inConf(configFile);
    if(inConf)
    {
        std::string configLine;
        getline(inConf, configLine);
        while(inConf.good() && (configLine.size() == 0 || configLine[0] == '#') )
        {
            getline(inConf, configLine);
        }
        int width, height;
        bool fullscreen;
        {
            std::stringstream inConfigLine;
            inConfigLine << configLine;
            inConfigLine >> width >> height >> fullscreen;
        }
        wWidth->setValue(width);
        wHeight->setValue(height);
        wFullscreen->setCheckState(fullscreen?Qt::Checked:Qt::Unchecked);

        while(inConf.good())
        {
            getline(inConf, configLine);
            if(configLine.size() != 0 && configLine[0] != '#')
            {
                std::string freq, screenFreq, x, y, size, r, g, b, full;
                std::stringstream inConfigLine;
                inConfigLine << configLine;
                inConfigLine >> freq >> screenFreq >> x >> y >> size >> r >> g >> b >> full;

                AddRow(freq.c_str(), screenFreq.c_str(), x.c_str(), y.c_str(), size.c_str());

                std::stringstream colorString;
                {
                    std::stringstream rstring;
                    rstring << r;
                    rstring >> m_r;
                    std::stringstream gstring;
                    gstring << g;
                    gstring >> m_g;
                    std::stringstream bstring;
                    bstring << b;
                    bstring >> m_b;
                }
                colorString << "Current color: ( " << m_r << " , " << m_g << " , " << m_b << " )";
                currColorLabel->setText(colorString.str().c_str());

                {
                    bool fullB;
                    std::stringstream fullstring;
                    fullstring << full;
                    fullstring >> fullB;
                    fillSquaresChecker->setCheckState(fullB?Qt::Checked:Qt::Unchecked); 
                }
            }
        }
    }
}

void ConfigUtil::AddRow(const char * frequency, const char * screenFreq, 
    const char * x, const char * y, const char * size)
{
    int newRow = squaresTable->rowCount();
    squaresTable->insertRow(squaresTable->rowCount());
    // Freq
    QTableWidgetItem * newItem = new QTableWidgetItem(frequency);
    newItem->setTextAlignment(Qt::AlignCenter);
    squaresTable->setItem(newRow, 0, newItem);
    // Screen freq
    newItem = new QTableWidgetItem(screenFreq);
    newItem->setTextAlignment(Qt::AlignCenter);
    squaresTable->setItem(newRow, 1, newItem);
    // X 
    newItem = new QTableWidgetItem(x);
    newItem->setTextAlignment(Qt::AlignCenter);
    squaresTable->setItem(newRow, 2, newItem);
    // Y
    newItem = new QTableWidgetItem(y);
    newItem->setTextAlignment(Qt::AlignCenter);
    squaresTable->setItem(newRow, 3, newItem);
    // Size
    newItem = new QTableWidgetItem(size);
    newItem->setTextAlignment(Qt::AlignCenter);
    squaresTable->setItem(newRow, 4, newItem);
}
    
void ConfigUtil::on_exitButton_clicked()
{
    SaveToConfig();
    qApp->quit();
}

void ConfigUtil::on_applyButton_clicked()
{
    SaveToConfig();
}

void ConfigUtil::on_addButton_clicked()
{
    AddRow("0", "60", "0", "0", "0");
}

void ConfigUtil::on_editColorButton_clicked()
{
    QColor color;
    color = QColorDialog::getColor();
    if(color.isValid())
    {
        std::stringstream colorString;
        m_r = color.red();
        m_g = color.green();
        m_b = color.blue();
        colorString << "Current color: ( " << m_r << " , " << m_g << " , " << m_b << " )";
        currColorLabel->setText(colorString.str().c_str());
    }
}

void ConfigUtil::SaveToConfig()
{
    std::ofstream outConf(m_configFile);
    outConf << wWidth->value() << " " << wHeight->value() << " " << (wFullscreen->checkState() == 2) << std::endl;
    for(int i = 0; i < squaresTable->rowCount(); ++i)
    {
        outConf << squaresTable->item(i, 0)->text().toStdString() << " " 
                     << squaresTable->item(i, 1)->text().toStdString() << " "
                     << squaresTable->item(i, 2)->text().toStdString() << " "
                     << squaresTable->item(i, 3)->text().toStdString() << " "
                     << squaresTable->item(i, 4)->text().toStdString() << " "
                     << m_r << " " << m_g << " " << m_b << " "
                     << (fillSquaresChecker->checkState() == 2) << std::endl;
    }
    outConf.close();
    std::cout << "Saved new config in config file!" << std::endl;
}


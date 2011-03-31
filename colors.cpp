#include "colors.h"
#include "ui_colors.h"

Colors::Colors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Colors)
{
    ui->setupUi(this);
    backgroundColor_ = "Black";
    fontColor_ = "White";
    ui->testButton->setStyleSheet("QPushButton {background-color: Black;color: white;}");

    ui->textWidget->setSortingEnabled(true);
    ui->backgroundWidget->setSortingEnabled(true);

    ui->backgroundWidget->addItem(new QListWidgetItem("Aqua"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Red"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Black"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Crimson"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Chocolate"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DarkMagenta"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DeepPink"));
    ui->backgroundWidget->addItem(new QListWidgetItem("ForestGreen"));
    ui->backgroundWidget->addItem(new QListWidgetItem("White"));
    ui->backgroundWidget->addItem(new QListWidgetItem("PapayaWhip"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Orange"));
    ui->backgroundWidget->addItem(new QListWidgetItem("OrangeRed"));
    ui->backgroundWidget->addItem(new QListWidgetItem("PowderBlue"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Purple"));
    ui->backgroundWidget->addItem(new QListWidgetItem("SeaGreen"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Turquoise"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Teal"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Yellow"));
    ui->backgroundWidget->addItem(new QListWidgetItem("MidnightBlue"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Navy"));
    ui->backgroundWidget->addItem(new QListWidgetItem("MediumSpringGreen"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DodgerBlue"));
    ui->backgroundWidget->addItem(new QListWidgetItem("SkyBlue"));
    ui->backgroundWidget->addItem(new QListWidgetItem("MediumPurple"));
    ui->backgroundWidget->addItem(new QListWidgetItem("GreenYellow"));
    ui->backgroundWidget->addItem(new QListWidgetItem("MediumOrchid"));
    ui->backgroundWidget->addItem(new QListWidgetItem("ForestGreen"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DarkKhaki"));
    ui->backgroundWidget->addItem(new QListWidgetItem("MediumVioletRed"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Lavender"));
    ui->backgroundWidget->addItem(new QListWidgetItem("LightCoral"));
    ui->backgroundWidget->addItem(new QListWidgetItem("PowderBlue"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Khaki"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Wheat"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Darkorange"));
    ui->backgroundWidget->addItem(new QListWidgetItem("PeachPuff"));
    ui->backgroundWidget->addItem(new QListWidgetItem("MistyRose"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Salmon"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Blue"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DarkRed"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Indigo"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DarkSlateGray"));
    ui->backgroundWidget->addItem(new QListWidgetItem("DimGray"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Gold"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Gray"));
    ui->backgroundWidget->addItem(new QListWidgetItem("IndianRed"));
    ui->backgroundWidget->addItem(new QListWidgetItem("LimeGreen"));
    ui->backgroundWidget->addItem(new QListWidgetItem("Lime"));

    ui->textWidget->addItem(new QListWidgetItem("Aqua"));
    ui->textWidget->addItem(new QListWidgetItem("Red"));
    ui->textWidget->addItem(new QListWidgetItem("Black"));
    ui->textWidget->addItem(new QListWidgetItem("Crimson"));
    ui->textWidget->addItem(new QListWidgetItem("Chocolate"));
    ui->textWidget->addItem(new QListWidgetItem("DarkMagenta"));
    ui->textWidget->addItem(new QListWidgetItem("DeepPink"));
    ui->textWidget->addItem(new QListWidgetItem("ForestGreen"));
    ui->textWidget->addItem(new QListWidgetItem("White"));
    ui->textWidget->addItem(new QListWidgetItem("PapayaWhip"));
    ui->textWidget->addItem(new QListWidgetItem("Orange"));
    ui->textWidget->addItem(new QListWidgetItem("OrangeRed"));
    ui->textWidget->addItem(new QListWidgetItem("PowderBlue"));
    ui->textWidget->addItem(new QListWidgetItem("Purple"));
    ui->textWidget->addItem(new QListWidgetItem("SeaGreen"));
    ui->textWidget->addItem(new QListWidgetItem("Turquoise"));
    ui->textWidget->addItem(new QListWidgetItem("Teal"));
    ui->textWidget->addItem(new QListWidgetItem("Yellow"));
    ui->textWidget->addItem(new QListWidgetItem("MidnightBlue"));
    ui->textWidget->addItem(new QListWidgetItem("Navy"));
    ui->textWidget->addItem(new QListWidgetItem("MediumSpringGreen"));
    ui->textWidget->addItem(new QListWidgetItem("DodgerBlue"));
    ui->textWidget->addItem(new QListWidgetItem("SkyBlue"));
    ui->textWidget->addItem(new QListWidgetItem("MediumPurple"));
    ui->textWidget->addItem(new QListWidgetItem("GreenYellow"));
    ui->textWidget->addItem(new QListWidgetItem("MediumOrchid"));
    ui->textWidget->addItem(new QListWidgetItem("ForestGreen"));
    ui->textWidget->addItem(new QListWidgetItem("DarkKhaki"));
    ui->textWidget->addItem(new QListWidgetItem("MediumVioletRed"));
    ui->textWidget->addItem(new QListWidgetItem("Lavender"));
    ui->textWidget->addItem(new QListWidgetItem("LightCoral"));
    ui->textWidget->addItem(new QListWidgetItem("PowderBlue"));
    ui->textWidget->addItem(new QListWidgetItem("Khaki"));
    ui->textWidget->addItem(new QListWidgetItem("Wheat"));
    ui->textWidget->addItem(new QListWidgetItem("Darkorange"));
    ui->textWidget->addItem(new QListWidgetItem("PeachPuff"));
    ui->textWidget->addItem(new QListWidgetItem("MistyRose"));
    ui->textWidget->addItem(new QListWidgetItem("Salmon"));
    ui->textWidget->addItem(new QListWidgetItem("Blue"));
    ui->textWidget->addItem(new QListWidgetItem("DarkRed"));
    ui->textWidget->addItem(new QListWidgetItem("Indigo"));
    ui->textWidget->addItem(new QListWidgetItem("DarkSlateGray"));
    ui->textWidget->addItem(new QListWidgetItem("DimGray"));
    ui->textWidget->addItem(new QListWidgetItem("Gold"));
    ui->textWidget->addItem(new QListWidgetItem("Gray"));
    ui->textWidget->addItem(new QListWidgetItem("IndianRed"));
    ui->textWidget->addItem(new QListWidgetItem("LimeGreen"));
    ui->textWidget->addItem(new QListWidgetItem("Lime"));
}

Colors::~Colors()
{
    delete ui;
}

QString Colors::getFont() {
    return fontColor_;
}

QString Colors::getBackground(){
    return backgroundColor_;
}


void Colors::on_textWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    fontColor_ = current->text();
    QString coloredFont = fontColor_;
    coloredFont.append(";}");
    coloredFont.prepend("QPushButton {background-color:" + backgroundColor_ + ";color:");
    const QString color = coloredFont;
    ui->testButton->setStyleSheet(color);
}

void Colors::on_backgroundWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    backgroundColor_ = current->text();
    QString coloredFont = backgroundColor_;
    coloredFont.append(";color:" + fontColor_ + ";}");
    coloredFont.prepend("QPushButton {background-color:");
    const QString color = coloredFont;
    ui->testButton->setStyleSheet(color);
}

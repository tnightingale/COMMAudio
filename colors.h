#ifndef COLORS_H
#define COLORS_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
    class Colors;
}

class Colors : public QDialog
{
    Q_OBJECT

public:
    explicit Colors(QWidget *parent = 0);
    ~Colors();

    /**
     * returns font color
     *
     * @author Joel Stewart
     */
    QString getFont();
    /**
     * returns background color
     *
     * @author Joel Stewart
     */
    QString getBackground();
private slots:

    /**
     * tracks text color wanted
     *
     * @author Joel Stewart
     */
    void on_textWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

    /**
     * tracks background color wanted
     *
     * @author Joel Stewart
     */
    void on_backgroundWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
    Ui::Colors *ui;
    QString backgroundColor_;
    QString fontColor_;
};

#endif // COLORS_H

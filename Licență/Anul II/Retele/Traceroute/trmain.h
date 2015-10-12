#ifndef TRMAIN_H
#define TRMAIN_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#include <string>
#include <vector>

namespace Ui {
class trMain;
}

class trMain : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit trMain(QWidget *parent = 0);
    ~trMain();
    void init();
    void showError(int err);

private slots:
    void on_traceButton_clicked();

    void on_packetList_clicked(const QModelIndex &index);

private:
    Ui::trMain *ui;
};

#endif // TRMAIN_H

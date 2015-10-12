#include "trmain.h"
#include "ui_trmain.h"
#include <QMessageBox>
#include <QTableView>
#include <QItemDelegate>
#include <QStandardItemModel>

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
#include "traceroute.h"



trMain::trMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::trMain)
{
    ui->setupUi(this);   

    init();
}

QStandardItemModel *model;
int currentID = 0;

void trMain::init()
{
    ui->ttlVal->setText("35");
    ui->destAddr->setText("www.emag.ro");
    ui->attempts->setText("3");
    ui->status->setText("Not running");
    ui->timeout->setText("5");

    model = new QStandardItemModel(2,4,this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Sender")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Receiver")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Status")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Latency")));

    ui->packetList->setModel(model);
}


trMain::~trMain()
{
    delete ui;
}

vector<PACKET> packets;

void trMain::on_traceButton_clicked()
{
    QString temp;
    QString theTrace = "";

    int i;
    int latency;
    int maxTTL;
    int currentTrace = 1;
    ICMP_PACKET recv;
    ICMP_PACKET sentICMPPacket;
    UDP_PACKET sentUDPPacket;

    PACKET tempPK;

    QStandardItem *rID;
    QStandardItem *rSender;
    QStandardItem *rRecv;
    QStandardItem *rStatus;
    QStandardItem *rLatency;

    struct in_addr* in;
    int err;

    Traceroute* tracer;
    tracer = (Traceroute*)malloc(sizeof(Traceroute));
    tracer = new Traceroute();

    maxTTL = ui->ttlVal->toPlainText().toInt();
    if (ui->icmpButton->isChecked())
        tracer->useICMP();
    else
        tracer->useUDP();
    tracer->setTimeout(ui->timeout->toPlainText().toInt());
    tracer->setAttemptNr(ui->attempts->toPlainText().toInt());
    tracer->setDestinationAddr(ui->destAddr->toPlainText().toStdString());

    for (i=1; i<=maxTTL; i++)
    {
        tracer->setTTL(i);
        theTrace = ui->tracepath->toPlainText();

        err = tracer->trace();
        if (err == _TRACEROUTE_ALL_OK || err == _TRACEROUTE_PACKET_DIED || err == _TRACEROUTE_PACKET_LOST)
        {
            tracer->getRecvICMPPacket (&recv);
            tracer->getSentICMPPacket (&sentICMPPacket);
            tracer->getSentUDPPacket (&sentUDPPacket);

            latency = 0;

            if (ui->icmpButton->isChecked())
            {
                ui->status->setText("ICMP");

                tempPK.index = currentID;
                tempPK.icmpPacket = sentICMPPacket;
                tempPK.isIcmp = 1;
                tempPK.latency = 0;
                packets.insert(packets.end(), tempPK);

                //build table entry
                temp = QString("%1").arg(currentID);
                rID = new QStandardItem(temp);
                rLatency = new QStandardItem(QString("%1").arg(latency));
                if (err == _TRACEROUTE_PACKET_LOST)
                    rStatus = new QStandardItem(QString("Lost"));
                else
                if (err == _TRACEROUTE_PACKET_DIED)
                    rStatus = new QStandardItem(QString("Died"));
                else
                    rStatus = new QStandardItem(QString("Final"));

                in = (struct in_addr*)(&(sentICMPPacket.iph.saddr));
                rSender = new QStandardItem(QString(inet_ntoa(*in)));
                in = (struct in_addr*)(&(sentICMPPacket.iph.daddr));
                rRecv = new QStandardItem(QString(inet_ntoa(*in)));

                model->setItem(currentID, 0, rID);
                model->setItem(currentID, 1, rSender);
                model->setItem(currentID, 2, rRecv);
                model->setItem(currentID, 3, rStatus);
                model->setItem(currentID, 4, rLatency);
                currentID++;
            }
            else
            {
                ui->status->setText("UDP");

                tempPK.index = currentID;
                tempPK.udpPacket = sentUDPPacket;
                tempPK.isIcmp = 0;
                tempPK.latency = 0;
                packets.insert(packets.end(), tempPK);

                //build table entry
                temp = QString("%1").arg(currentID);
                rID = new QStandardItem(temp);
                rLatency = new QStandardItem(QString("%1").arg(latency));
                if (err == _TRACEROUTE_PACKET_LOST)
                    rStatus = new QStandardItem(QString("Lost"));
                else
                if (err == _TRACEROUTE_PACKET_DIED)
                    rStatus = new QStandardItem(QString("Died"));
                else
                    rStatus = new QStandardItem(QString("Final"));


                in = (struct in_addr*)(&(sentUDPPacket.iph.saddr));
                rSender = new QStandardItem(QString(inet_ntoa(*in)));
                in = (struct in_addr*)(&(sentUDPPacket.iph.daddr));
                rRecv = new QStandardItem(QString(inet_ntoa(*in)));

                model->setItem(currentID, 0, rID);
                model->setItem(currentID, 1, rSender);
                model->setItem(currentID, 2, rRecv);
                model->setItem(currentID, 3, rStatus);
                model->setItem(currentID, 4, rLatency);
                currentID++;
            }

            latency = tracer->getLatencyMs();
            tempPK.index = currentID;
            tempPK.icmpPacket = recv;
            tempPK.isIcmp = 1;
            tempPK.latency = latency;
            packets.insert(packets.end(), tempPK);

            //build table entry
            temp = QString("%1").arg(currentID);
            rID = new QStandardItem(temp);
            rStatus = new QStandardItem(QString("received"));
            rLatency = new QStandardItem(QString("%1").arg(latency));

            in = (struct in_addr*)(&(recv.iph.saddr));
            rSender = new QStandardItem(QString(inet_ntoa(*in)));

            if (err != _TRACEROUTE_PACKET_LOST)
            {
                theTrace.append(QString("%1. ").arg(currentTrace));
                theTrace.append(QString("%1 - ").arg(inet_ntoa(*in)));
                theTrace.append(QString("TTL: %1 - ").arg(i));
                theTrace.append(QString("Latency: %1 Ms\n").arg(latency));
            }
            else
                theTrace.append(QString("%1. Packet timeout").arg(currentTrace));

            in = (struct in_addr*)(&(recv.iph.daddr));
            rRecv = new QStandardItem(QString(inet_ntoa(*in)));

            model->setItem(currentID, 0, rID);
            model->setItem(currentID, 1, rSender);
            model->setItem(currentID, 2, rRecv);
            model->setItem(currentID, 3, rStatus);
            model->setItem(currentID, 4, rLatency);
            currentID++;

            ui->packetList->setModel(model);
            ui->tracepath->setPlainText(theTrace);
            currentTrace++;
        }
        else
        {
            showError(err);
        }

        if (err == _TRACEROUTE_ALL_OK) break;
    }
}

void trMain::showError(int err)
{
    string error;

    switch (err)
    {
        case -2:
            error="Nothin' happenin'\n";
            break;
        case -3:
            error="All ok\n";
            break;
        case -4:
            error="Packet died\n";
            break;
        case -5:
            error="Send error\n";
            break;
        case -6:
            error="Socket error\n";
            break;
        case -7:
            error="Packet lost\n";
            break;
        case -8:
            error="Packet read error\n";
            break;
        case -9:
            error="Received my own package\n";
            break;
        default:
            break;
    }

    ui->status->setText(error.data());
}



void trMain::on_packetList_clicked(const QModelIndex &index)
{
    PACKET p;
    iphdr iph;
    udphdr udph;
    icmphdr icmph;
    p = packets[index.row()];
    struct in_addr* in;

    QString info = "";
    if (p.isIcmp)
    {
        info.append("ICMP PACKET - Latency: ");
        iph = p.icmpPacket.iph;
    }
    else
    {
        info.append("UDP PACKET - Latency: ");
        iph = p.udpPacket.iph;
    }
    info.append(QString("%1 Ms\n").arg(p.latency));
    info.append("----------------\n");
    info.append("IP Header\n");
    info.append(QString("-->Version: IPv%1\n").arg(iph.version));
    info.append(QString("-->TOS: %1\n").arg(iph.tos));
    info.append(QString("-->Total Packet Size: %1\n").arg(iph.tot_len));
    info.append(QString("-->ID: %1\n").arg(iph.id));
    info.append(QString("-->Frag: %1\n").arg(iph.frag_off));
    info.append(QString("-->TTL: %1\n").arg(iph.ttl));
    info.append(QString("-->Protocol: %1\n").arg(iph.protocol));
    in = (struct in_addr*)(&(iph.saddr));
    info.append(QString("-->Sender: %1\n").arg(inet_ntoa(*in)));
    in = (struct in_addr*)(&(iph.daddr));
    info.append(QString("-->Receiver: %1\n").arg(inet_ntoa(*in)));
    info.append(QString("-->Checksum: %1\n").arg(iph.check));
    info.append("----------------\n");
    if (p.isIcmp)
    {
        info.append("ICMP Header\n");
        icmph = p.icmpPacket.icmph;
        info.append(QString("--->Type: %1\n").arg(icmph.type));
        info.append(QString("--->Code: %1\n").arg(icmph.code));
        info.append(QString("--->ID: %1\n").arg(icmph.un.echo.id));
        info.append(QString("--->Sequence: %1\n").arg(icmph.un.echo.sequence));
        info.append(QString("--->Checksum: %1\n").arg(icmph.checksum));
    }
    else
    {
        info.append("UDP PACKET - Latency: ");
        udph = p.udpPacket.udph;
        info.append(QString("--->Source port: %1\n").arg(udph.source));
        info.append(QString("--->Destination port: %1\n").arg(udph.dest));
        info.append(QString("--->UPDH len: %1\n").arg(udph.len));
    }

    ui->packetView->setPlainText(info);
}



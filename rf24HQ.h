/*-
 * Copyright (c) 2012 Darran Hunt (darran [at] hunt dot net dot nz)
 * All rights reserved.
 * Some parts copyright (c) 2012 Eric Brundick (spirilis [at] linux dot com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RF24_H_
#define _RF24_H_

#include <Arduino.h>
#include <avr/pgmspace.h>

#include <nRF24L01.h>

#define RF24_CONFIG      0x00
#define RF24_EN_AA       0x01
#define RF24_RF_CH       0x05
#define RF24_RF_SETUP    0x06
#define RF24_RPD         0x09

#define RF24_SPEED_250KBPS  0x02
#define RF24_SPEED_1MBPS    0x00
#define RF24_SPEED_2MBPS    0x01
#define RF24_SPEED_MAX      RF24_SPEED_2MBPS
#define RF24_SPEED_MIN      RF24_SPEED_250KBPS

#define RF24_POWER_0DBM        0x03
#define RF24_POWER_MINUS6DBM   0x02
#define RF24_POWER_MINUS12DBM  0x01
#define RF24_POWER_MINUS18DBM  0x00
#define RF24_POWER_MAX         RF24_POWER_0DBM
#define RF24_POWER_MIN         RF24_POWER_MINUS18DBM

#define RF24_ADDR_LEN 5
#define RF24_MAX_SIZE 32	/* Maximum message payload size */

#define RF24_STATE_NOTPRESENT  0x00
#define RF24_STATE_POWERDOWN   0x01
#define RF24_STATE_STANDBY_I   0x02
#define RF24_STATE_STANDBY_II  0x03
#define RF24_STATE_PTX         0x04
#define RF24_STATE_PRX         0x05
#define RF24_STATE_TEST        0x06

#define RF24_IRQ_TXFAILED      0x01
#define RF24_IRQ_TX            0x02
#define RF24_IRQ_RX            0x04

class RFDebug : public Print {
public:
    RFDebug();
    void begin(Print *debugPrint);
    virtual size_t write(uint8_t byte);
private:
    Print *debug;
};

class rf24 {
public:
    rf24(uint8_t cePin=8, uint8_t csnPin=9, uint8_t channel=80, uint8_t payload=RF24_MAX_SIZE);
    
    boolean begin(Print *debugPrint=NULL);
    boolean begin(uint32_t dataRate, Print *debugPrint);

    void chipEnable();
    void chipDisable();
    void chipSelect();
    void chipDeselect();

    uint8_t transfer(uint8_t data);
    void tx(const void *data, uint8_t len, uint8_t max=RF24_MAX_SIZE);
    void txlsbfirst(const void *data, uint8_t len);
    void rx(void *data, uint8_t len, uint8_t max=RF24_MAX_SIZE);
    void rxlsbfirst(void *data, uint8_t len, uint8_t max=RF24_MAX_SIZE);
    void txrx(uint8_t *data, uint8_t *in, uint8_t len, uint8_t max=RF24_MAX_SIZE);
    void readReg(uint8_t reg, void *value, uint8_t size=1);
    uint8_t readReg(uint8_t reg);
    void writeReg(uint8_t reg);
    void writeReg(uint8_t reg, uint8_t value);
    void writeReg(uint8_t reg, const void *value, uint8_t size);
    void setConfig(uint8_t value);
    void setIrqMask(uint8_t intcodes);

    void setCRC8(void);
    void setCRC16(void);
    void setCRCOn(void);
    void setCRCOff(void);

    void setRxAddr(uint8_t id, const void *addr);
    void disableQueue(uint8_t id);
    void setTxAddr(const void *addr);
    void setPacketSize(uint8_t size);
    uint8_t getPacketSize();
    void enableAck(uint16_t delay, uint8_t retry);
    void disableAck();
    boolean gotAck();

    void setChannel(uint8_t chan);
    uint8_t getChannel(void);
    void setSpeed(uint32_t rfspd);
    void setSpeedReg(uint8_t setting);
    void setPowerReg(uint8_t power);
    uint8_t getSpeedReg(void);
    uint32_t getSpeed(void);
    char* getSpeedString(char *buf);
    uint8_t getPowerReg(void);
    char* getPowerString(char *buf);
    uint8_t getRetransmits(void);
    uint8_t getFailedSends(void);
    void resetFailedSends(void);

    uint8_t chipState();
    uint8_t getReason();
    boolean getReason(uint8_t intcode);
    void standby();
    void enableTx();
    void continualTx();
    void enableRx();
    void powerDown();
    boolean isSending(bool enableReceive=true);


    void flushRx();
    void flushTx();
    boolean rxFifoAvailable();
    boolean available();
    boolean available(uint32_t timeout);
    void send(void *data, uint8_t size=RF24_MAX_SIZE);
    void read(void *data, uint8_t size=RF24_MAX_SIZE);
    uint8_t readQueue();
    boolean sendAndRead(void *msg, uint8_t size=RF24_MAX_SIZE, uint32_t timeout=100);

    void scan(uint8_t *chans, uint8_t start=0, uint8_t count=125, uint8_t depth=128);
    void setHandler(void (*rfHandler)(void *msg, uint8_t size), void *msg, uint8_t size);
    void disableHandler(void);
    void loop();

    void dumpRegisters(void);
    boolean isAlive(void);


    uint8_t cePin;
    uint8_t csnPin;
    uint8_t lastQueue;  // Data pipe of the last read() performed.
  private:
    RFDebug debug;	// debug print

    uint16_t _scrubDelay(uint16_t delay);
    uint8_t _convertSpeedToReg(uint32_t rfspd);
    uint32_t _convertRegToSpeed(uint8_t rfspdreg);

    uint8_t channel;
    uint8_t packetSize;
    boolean acked;
    boolean sending;
    boolean autoAck;	/* Auto-ack feature enabled */
    uint8_t cfg_crc;  /* 2-byte CRC enabled */
    uint8_t cfg_irq;  /* IRQ mask */
    uint8_t rfspeed;  /* Data rate; see RF24_SPEED_* defines */
    uint8_t rfpower;  /* Transmitter power; see RF24_POWER_* defines */

    void (*handler)(void *msg, uint8_t size);
    void *handlerMsg;
    uint8_t handlerMsgSize;
};

#define RFSTREAM_SIZE (RF24_MAX_SIZE-1)

typedef struct {
    uint8_t size;
    uint8_t data[RF24_MAX_SIZE-1];
} rfstream_buf_t;

class RFStream : public Stream {
public:
    RFStream();
    void begin(rf24 *rf, Print *debugPrint=NULL);
    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    virtual int peek();
    void setFlushTime(uint16_t msecs);
    using Print::write;
    rf24 *rf;
private:
    RFDebug debug;
    rfstream_buf_t txBuf;
    rfstream_buf_t rxBuf;
    uint8_t rxInd;
    uint32_t lastWrite;
    uint16_t flushTime;
    bool sending;
};
#endif

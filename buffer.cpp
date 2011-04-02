#include "buffer.h"

Buffer::Buffer() : readPosition_(0), writePosition_(0) { }

Buffer::Buffer(const QByteArray& data) 
: readPosition_(0), writePosition_(0), data_(data) { }

unsigned char Buffer::readByte()
{
    if (bytesAvailable() < 1) {
        throw new EOFException();
    }

    return data_.constData()[readPosition_++];
}

short Buffer::readShort()
{
    if (bytesAvailable() < sizeof(short)) {
        throw new EOFException();
    }

    short ret = *(short*)&data_.constData()[readPosition_];
    readPosition_ += sizeof(short);
    return ret;
}

int Buffer::readInt()
{
    if (bytesAvailable() < sizeof(int)) {
        throw new EOFException();
    }

    int ret = *(int*)&data_.constData()[readPosition_];
    readPosition_ += sizeof(int);
    return ret;
}

float Buffer::readFloat()
{
    if (bytesAvailable() < sizeof(float)) {
        throw new EOFException();
    }

    float ret = *(float*)&data_.constData()[readPosition_];
    readPosition_ += sizeof(float);
    return ret;
}

double Buffer::readDouble()
{
    if (bytesAvailable() < sizeof(double)) {
        throw new EOFException();
    }

    double ret = *(double*)&data_.constData()[readPosition_];
    readPosition_ += sizeof(double);
    return ret;
}

QByteArray Buffer::read(const int maxLength)
{
    int numRead = 0;
    if ((numRead = bytesAvailable()) > maxLength) {
        numRead = maxLength;
    }

    QByteArray ret = data_.mid(readPosition_, numRead);
    readPosition_ += ret.size();
    return ret;
}

void Buffer::writeByte(const unsigned char value)
{
    data_ += value;
    writePosition_++;
}

void Buffer::writeShort(const short value)
{
    data_ += QByteArray::fromRawData((const char*)&value, sizeof(short));
    writePosition_ += sizeof(short);
}

void Buffer::writeInt(const int value)
{
    data_ += QByteArray::fromRawData((const char*)&value, sizeof(int));
    writePosition_ += sizeof(int);
}

void Buffer::writeFloat(const float value)
{
    data_ += QByteArray::fromRawData((const char*)&value, sizeof(float));
    writePosition_ += sizeof(float);
}

void Buffer::writeDouble(const double value)
{
    data_ += QByteArray::fromRawData((const char*)&value, sizeof(double));
    writePosition_ += sizeof(double);
}

void Buffer::write(const QByteArray& value)
{
    data_ += value;
    writePosition_ += value.size();
}

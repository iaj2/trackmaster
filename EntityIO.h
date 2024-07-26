#ifndef ENTITYIO_H
#define ENTITYIO_H

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

template<typename T>
class EntityIO {
public:
    EntityIO(const std::string& filename);
    ~EntityIO();

    void openFile();
    void closeFile();
    
    void seekToStart();
    void seekTo(int offset);
    std::vector<T*> readNRecords(int n);
    T* readRecord();
    void appendRecord(const T& record);
    int getRecordCount();

private:
    std::fstream fileStream;
    std::string filename;
    int recordCount;

};

#endif // ENTITYIO_H

#pragma once

#include <QObject>
#include "Model/systemcontroller.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

namespace GUI{
    class BeBoardRegistersWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit BeBoardRegistersWorker(QObject *parent,
                                        SystemController &sysCtrl);
        ~BeBoardRegistersWorker();

        void requestWork();

    signals:
        void workRequested();
        void sendInitialBeBoardRegValues(const int idSh, const int idBe, const std::map< std::string, uint32_t >  cMap);
        void finished();
        void finishedInitialiseBeBoardRegValues();

    public slots:
        void getInitialBeBoardRegMap();

    private:

        SystemController& m_systemController;

        BeBoardInterface*       fBeBoardInterface;
        CbcInterface*           fCbcInterface;
        ShelveVec fShelveVector;
        BeBoardFWMap fBeBoardFWMap;

        void getObjects();


        explicit BeBoardRegistersWorker(const BeBoardRegistersWorker& rhs) = delete;
        BeBoardRegistersWorker& operator= (const BeBoardRegistersWorker& rhs) = delete;
    };
}

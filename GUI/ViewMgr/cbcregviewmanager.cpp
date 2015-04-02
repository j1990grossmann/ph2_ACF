#include "cbcregviewmanager.h"
#include "View/cbcregisterstab.h"
#include "Model/cbcregisters.h"

#include <QDebug>

namespace GUI
{
    CbcRegViewManager::CbcRegViewManager(QObject *parent,
                                         CbcRegistersTab& cbcRegTab,
                                         CbcRegisters& cbcReg) :
        QObject(parent),
        m_cbcRegistersTab(cbcRegTab),
        m_cbcRegisters(cbcReg)
    {
        WireConnections();
        WireExternalConnections();
    }

    CbcRegViewManager::~CbcRegViewManager()
    {
        qDebug() << "Destructing " << this;
    }

    void CbcRegViewManager::WireConnections()
    {
        connect(&m_cbcRegisters, SIGNAL(sendInitialCbcRegisterValue(int,int,int,int,std::map<std::string,CbcRegItem>)),
                &m_cbcRegistersTab, SLOT(createCbcRegisterValue(int,int,int,int,std::map<std::string,CbcRegItem>)));
        connect(&m_cbcRegistersTab, SIGNAL(writeCbcRegisters(int,std::vector<std::pair<std::string,std::uint8_t> >)),
                &m_cbcRegisters, SIGNAL(writeCbcRegisterValue(int,std::vector<std::pair<std::string,std::uint8_t> >)));

        connect(&m_cbcRegistersTab, SIGNAL(refreshCbcRegisters()),
                &m_cbcRegisters, SIGNAL(getCbcRegistersMap()));

        connect(&m_cbcRegisters, SIGNAL(sendCbcRegisterValue(int,std::map<std::string,CbcRegItem>)),
                &m_cbcRegistersTab, SLOT(updateCbcRegisterValues(int,std::map<std::string,CbcRegItem>)));

    }

    void CbcRegViewManager::WireExternalConnections()
    {
        connect(this, SIGNAL(sendInitialiseRegistersView()),
                &m_cbcRegisters, SIGNAL(createInitialCbcRegistersMap()));
        connect(&m_cbcRegisters, SIGNAL(finishedInitCbcReg()),
                this, SIGNAL(notifyConfigFinished()));

        connect(this, SIGNAL(sendSh(int)),
                &m_cbcRegistersTab, SLOT(setupShTab(int)));
        connect(this, SIGNAL(sendBe(int,int)),
                &m_cbcRegistersTab, SLOT(setupBeTab(int,int)));
        connect(this, SIGNAL(sendFe(int,int,int)),
                &m_cbcRegistersTab, SLOT(setupFeTab(int,int,int)));
        connect(this, SIGNAL(sendCbc(int,int,int,int)),
                &m_cbcRegistersTab, SLOT(setupCbcRegGrid(int,int,int,int)));

    }

}

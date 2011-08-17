/****************************************************************************
** Copyright (C) 2010-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Soap library.
**
** Licensees holding valid commercial KD Soap licenses may use this file in
** accordance with the KD Soap Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/
#ifndef KDSOAPSERVEROBJECTINTERFACE_H
#define KDSOAPSERVEROBJECTINTERFACE_H

#include "KDSoapServerGlobal.h"
#include <KDSoapMessage.h>
#include <QtCore/QObject>

/**
 * Base class for server objects, i.e. objects implementing the methods
 * that can be called by SOAP clients.
 *
 * Your server object must derive from both QObject (directly or indirectly)
 * and from KDSoapServerObjectInterface.
 * The virtual method processRequest is called whenever a SOAP request is being made.
 * To handle the call, either reimplement processRequest and do the dispatching
 * manually (not recommended), or use kdwsdl2cpp to generate the base class for your server object.
 *
 * Example for manual handling:
  <code>
  class EmployeeServerObject : public QObject, public KDSoapServerObjectInterface
  {
    Q_OBJECT
    Q_INTERFACES(KDSoapServerObjectInterface)

  public: // methods published to SOAP
    QString getEmployeeCountry(const QString& employeeName);
    [...]
  };
  </code>
  And in the .cpp file:
  <code>
    #define KDWSDLServerClass EmployeeServerObject
    #include "swsdl_employee.cpp"
  </code>
 *
 * swsdl_employee.cpp will contain the code which calls getEmployeeCountry
 * when parsing a KDSoapMessage that is a "getEmployeeCountry" request.
 */
class KDSOAPSERVER_EXPORT KDSoapServerObjectInterface
{
public:
    /**
     * Constructor
     */
    KDSoapServerObjectInterface();
    /**
     * Destructor
     */
    virtual ~KDSoapServerObjectInterface();

    /**
     * Handle \p request and return \p response.
     * The default implementation in this base class is to simply return
     * a fault "method not found". Subclasses must implement the dispatching
     * to actual implementation methods.
     */
    virtual void processRequest(const KDSoapMessage& request, KDSoapMessage& response, const QByteArray& soapAction);

    /**
     * Returns the SOAP headers that were provided together with the SOAP request.
     * This can be used to retrieve authentication headers, or any kind of session
     * (per-client) data.
     */
    KDSoapHeaders requestHeaders() const;

    /**
     * Returns the "Soap Action" header sent by the client.
     */
    QByteArray soapAction() const;

    /**
     * Sets the soap headers to be sent in the response
     */
    void setResponseHeaders(const KDSoapHeaders& headers);

    /**
     * Instructs KD SOAP to return a fault message instead of the return value of the slot.
     *
     * \param faultCode A code for identifying the fault. Example: "Server.EntryNotFound", or
     *                  "Client.Authentication". Must not be empty.
     * \param faultString A human readable explanation of the fault
     * \param faultActor Information about who caused the fault to happen
     * \param Holds application specific error information related to the Body element
     *
     * See http://www.w3.org/TR/2000/NOTE-SOAP-20000508/#_Toc478383507 for more details.
     */
    void setFault(const QString& faultCode, const QString& faultString, const QString& faultActor, const QString& detail);

    /**
     * Returns true if setFault was called in the current method invocation.
     */
    bool hasFault() const;

private:
    friend class KDSoapServerSocket;
    void setRequestHeaders(const KDSoapHeaders& headers, const QByteArray& soapAction);
    KDSoapHeaders responseHeaders() const;
    void storeFaultAttributes(KDSoapMessage& message) const;
    class Private;
    Private* const d;
};

Q_DECLARE_INTERFACE(KDSoapServerObjectInterface,
                    "com.kdab.KDSoap.ServerObjectInterface/1.0")

#endif // KDSOAPSERVEROBJECTINTERFACE_H

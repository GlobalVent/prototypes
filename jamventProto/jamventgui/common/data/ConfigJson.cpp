#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "ConfigJson.h"

namespace
{
    constexpr const char* ConfigFile = "Config.json";
}

ConfigJson::ConfigJson()
{
}

ConfigJson::~ConfigJson()
{
}

const ConfigJson::ConfigData& ConfigJson::getConfigData()
{
    return m_data;
}

void ConfigJson::parseConfigFile()
{
  QString filePath = QCoreApplication::applicationDirPath() + "/" + ConfigFile;

  QFile jsonFile(filePath);
  if (! jsonFile.exists())
  {
    qDebug() << "JSON file does not exist. file: " << filePath;
    return;
  }

  if (! jsonFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Failed to open JSON file: " << filePath;
    return;
  }

  QJsonParseError jsonError;
  QByteArray      arr = jsonFile.readAll();
  QJsonDocument   doc(QJsonDocument::fromJson(arr, &jsonError));
  jsonFile.close();

  if (jsonError.error != QJsonParseError::NoError)
  {
    qDebug() << "Failed to parse JSON file. Error: " << jsonError.errorString() << ", File: " << filePath;
    return;
  }

  QJsonObject topObj = doc.object();
  if (topObj.isEmpty())
  {
      qDebug() << "Top JSON object is emtpy.";
      return;
  }

  QJsonValue val = topObj.value("comm-mode");
  if ((! val.isUndefined()) && val.isString())
  {
      m_data.commMode = val.toString();
  }

  val = topObj.value("debug-mode");
  if ((!val.isUndefined()) && val.isString())
  {
      m_data.debugMode = val.toString();
  }

  val = topObj.value("graph-tick-count");
  if ((!val.isUndefined()) && val.isDouble())
  {
      m_data.graphTickCount = static_cast<int>(val.toDouble());
  }

  val = topObj.value("polling-timeout-ms");
  if ((!val.isUndefined()) && val.isDouble())
  {
      m_data.pollingTimeout_ms = val.toDouble();
  }

  val = topObj.value("graphs");
  if ((! val.isUndefined()) && val.isArray())
  {
      QJsonArray arr = val.toArray();
      for (int i = 0; i < arr.count(); ++i)
      {
          QJsonValue objVal = arr.at(i);
          if ((! objVal.isUndefined()) && objVal.isObject())
          {
              QJsonObject obj = objVal.toObject();

              m_data.graphs[i].isSet = true;

              QJsonValue pairVal = obj.value("x-axis-min");
              if ((! pairVal.isUndefined()) && pairVal.isDouble())
              {
                   m_data.graphs[i].xAxisMin = pairVal.toDouble();
              }

              pairVal = obj.value("x-axis-max");
              if ((! pairVal.isUndefined()) && pairVal.isDouble())
              {
                   m_data.graphs[i].xAxisMax = pairVal.toDouble();
              }

              pairVal = obj.value("y-axis-min");
              if ((! pairVal.isUndefined()) && pairVal.isDouble())
              {
                   m_data.graphs[i].yAxisMin = pairVal.toDouble();
              }

              pairVal = obj.value("y-axis-max");
              if ((! pairVal.isUndefined()) && pairVal.isDouble())
              {
                   m_data.graphs[i].yAxisMax = pairVal.toDouble();
              }

              pairVal = obj.value("y-axis-label");
              if ((! pairVal.isUndefined()) && pairVal.isString())
              {
                   m_data.graphs[i].yAxisLabel = pairVal.toString();
              }
          }
      }
   }
}

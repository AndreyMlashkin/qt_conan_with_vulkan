/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QApplication>
#include <QVulkanInstance>
#include <QLoggingCategory>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "shared/trianglerenderer.h"

Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")


class VulkanTestWindow : public QVulkanWindow
{
public:
  VulkanTestWindow()
    : instance(new QVulkanInstance())
  {
    instance->setLayers({ "VK_LAYER_KHRONOS_validation" });

    if (!instance->create())
      qFatal("Failed to create Vulkan instance: %d", instance->errorCode());

    setVulkanInstance(instance);
  }

  QVulkanWindowRenderer *createRenderer() override;

private:
  QVulkanInstance* instance {nullptr};
};

class WindowWrapper : public QWidget
{
public:
  WindowWrapper(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::Widget)
    : QWidget(parent, f)
  {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
  }

  void addWindow(QWindow* w)
  {
    QWidget* wrappedWindow = QWidget::createWindowContainer(w);
    addWidget(wrappedWindow);
  }

  void addWidget(QWidget* w)
  {
    layout()->addWidget(w);
  }
};

QVulkanWindowRenderer *VulkanTestWindow::createRenderer()
{
  return new TriangleRenderer(this, true); // try MSAA, when available
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

  WindowWrapper wrapper;

  // Widget
  QWidget child;
  child.setAutoFillBackground(true);
  QPalette pal = child.palette();
  pal.setColor(QPalette::Window, Qt::green);
  child.setPalette(pal);

  child.setMinimumSize(QSize(100,100));

  wrapper.addWidget(&child);
  // End widget

  VulkanTestWindow vulkanChild;

  wrapper.addWindow(&vulkanChild);
  wrapper.setMinimumSize(QSize(500,500));

  wrapper.show();

  return app.exec();
}

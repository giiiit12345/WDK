#pragma once

//json_operation.h
#ifndef JSON_OPERATION_H
#define JSON_OPERATION_H

#include <stdio.h>
#include <wx/wx.h>
#include <fstream>

#include "json.h"
#include "Symbol.h"

using namespace std;


extern Json::Value root;


//读取json文件中所有信息，返回Json::Value类型的数据
Json::Value ReadJson(const string filename);

//读取某个函数中颜色的属性，传入的为["pen"]["color"]一级的数据
wxColour ReadColorFromJson(Json::Value penValue);


//绘制图形
void DrawFromJson(Json::Value root, wxMemoryDC& memDC, int x, int y);

//导出为JSON文件
void WriteJson(Json::Value root, const std::string& newFilename);

void reWriteJson(const std::string& originalFilename, const std::string& newFilename);

//导出为Net文件
void WriteNet(const std::string& originalFilename, const std::string& newFilename);

#endif

class json
{
};



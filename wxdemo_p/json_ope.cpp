#include <wx/wx.h>
#include "json.h"
#include "jsoncpp.cpp"
#include "json_ope.h"
#include "Symbol.h"

//全局变量，读取一个名为的JSON文件,并将其内容存储在一个Json::Value对象root中
Json::Value root = ReadJson("test.json");

//读取json文件中所有信息,将文件内容解析到一个Json::Value对象中
Json::Value ReadJson(const string filename)
{
    Json::Reader reader;
    Json::Value root;
    ifstream file(filename, ios::binary);

    if (reader.parse(file, root)) {
        return root;
    }
    else {
        wxLogError("无法解析JSON文件: %s", "config.json");
        return Json::Value::null;
    }
}



//读取某个函数中颜色的属性，传入的为["pen"]["borderColor"]一级的数据
wxColour ReadColorFromJson(Json::Value penValue)
{
    wxColor color;
    if (penValue.isString()) {
        const string strColor = penValue.asString();
        if (strColor == "GREEN") {
            color = GREEN;
        }
        else if (strColor == "RED") {
            color = RED;
        }
        else if (strColor == "YELLOW") {
            color = YELLOW;
        }
        else if (strColor == "WHITE") {
            color = WHITE;
        }
        else if (strColor == "BLACK") {
            color = wxColour(0, 0, 0);
        }
        else if (strColor == "BLUE") {
            color = wxColour(0, 0, 255);
        }
        else {
            return wxNullColour;
        }
    }
    else if (penValue.isArray()) {
        color = wxColor(penValue[0].asInt(), penValue[1].asInt(), penValue[2].asInt());
    }
    return color;
}



//绘制图形
void DrawFromJson(Json::Value root, wxMemoryDC& memDC, int x, int y)
{
    //存储json文件数据
    const Json::Value& functions = root["functions"];

    //绘制矩形
    for (const Json::Value& functionData : functions["rectangle"]) {
        int x1 = functionData["factor"][0].asInt();
        int y1 = functionData["factor"][1].asInt();
        int x2 = functionData["factor"][2].asInt();
        int y2 = functionData["factor"][3].asInt();

        //读取绘制工具相关参数
        wxColor borderColor = ReadColorFromJson(functionData["pen"]["color"]);
        DrawRectangle(memDC, x + x1, y + y1, x + x2, y + y2, borderColor);
    }

    //绘制三角形
    for (const Json::Value& functionData : functions["triangle"]) {
        int x1 = functionData["factor"][0].asInt();
        int y1 = functionData["factor"][1].asInt();
        int x2 = functionData["factor"][2].asInt();
        int y2 = functionData["factor"][3].asInt();
        int x3 = functionData["factor"][4].asInt();
        int y3 = functionData["factor"][5].asInt();

        //读取绘制工具相关参数
        wxColor borderColor = ReadColorFromJson(functionData["pen"]["color"]);
        DrawTriangle(memDC, x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, borderColor);
    }

    //绘制文本
    for (const Json::Value& functionData : functions["text"]) {
        std::string content = functionData["content"].asString();
        int x1 = functionData["factor"][0].asInt();
        int y1 = functionData["factor"][1].asInt();
        double angle = functionData["factor"][2].asDouble();

        //读取绘制工具相关参数
        wxColor color = ReadColorFromJson(functionData["color"]);

        DrawText(memDC, content.c_str(), x + x1, y + y1, color);
    }

    //绘制箭头
    for (const Json::Value& functionData : functions["arrow"]) {
        int x1 = functionData["factor"][0].asInt();
        int y1 = functionData["factor"][1].asInt();
        int x2 = functionData["factor"][2].asInt();
        int y2 = functionData["factor"][3].asInt();

        //读取绘制工具相关参数
        wxColor color = ReadColorFromJson(functionData["pen"]["color"]);

        DrawArrow(memDC, x + x1, y + y1, x + x2, y + y2, color);
    }

    //绘制圆
    for (const Json::Value& functionData : functions["circle"]) {
        int x1 = functionData["factor"][0].asInt();
        int y1 = functionData["factor"][1].asInt();
        double radius = functionData["factor"][2].asDouble();

        //读取绘制工具相关参数
        wxColor borderColor = ReadColorFromJson(functionData["pen"]["color"]);

        DrawCircle(memDC, x + x1, y + y1, radius, borderColor);
    }

    //绘制半圆
    for (const Json::Value& functionData : functions["arc"]) {
        int x1 = functionData["factor"][0].asInt(); // 
        int y1 = functionData["factor"][1].asInt(); // 圆心坐标
        int radiusX = functionData["factor"][2].asInt(); // 半径
        bool drawLeftHalf = functionData["factor"][3].asBool();

        // 读取绘制工具相关参数
        wxColour borderColor = ReadColorFromJson(functionData["pen"]["color"]);

        DrawHalfCircle(memDC, x + x1, y + y1, radiusX, borderColor, drawLeftHalf);
    }

    //绘制半椭圆
    for (const Json::Value& functionData : functions["semiellipse"]) {
        int x1 = functionData["factor"][0].asInt(); // 
        int y1 = functionData["factor"][1].asInt(); // 圆心坐标
        int radiusX = functionData["factor"][2].asInt(); // x方向半径
        int radiusY = functionData["factor"][3].asInt(); // y方向半径
        bool drawLeftHalf = functionData["factor"][3].asBool();

        // 读取绘制工具相关参数
        wxColour borderColor = ReadColorFromJson(functionData["pen"]["color"]);

        DrawHalfEllipse(memDC, x + x1, y + y1, radiusX, radiusY, borderColor, drawLeftHalf);
    }


    //绘制直线
    for (const Json::Value& functionData : functions["line"]) {
        int x1 = functionData["factor"][0].asInt();
        int y1 = functionData["factor"][1].asInt();
        int x2 = functionData["factor"][2].asInt();
        int y2 = functionData["factor"][3].asInt();

        //读取绘制工具相关参数
        wxColor color = ReadColorFromJson(functionData["pen"]["color"]);
        DrawLine(memDC, x + x1, y + y1, x + x2, y + y2, color);
    }
}
void WriteJson(const Json::Value root, const std::string& newFilename)
{
    // 创建一个临时对象，复制传入的 data
    Json::Value modifiedData = root;

    // 移除position属性
    if (modifiedData.isMember("position"))
    {
        modifiedData.removeMember("position");
    }

    // 打开文件，检查是否有内容
    std::ifstream checkFile(newFilename);
    bool fileNotEmpty = checkFile.peek() != std::ifstream::traits_type::eof();
    checkFile.close();

    // 创建一个 Json::Value 对象，代表整个文件内容
    Json::Value fileContent;

    // 如果文件非空，读取文件内容到 fileContent
    if (fileNotEmpty)
    {
        std::ifstream existingFile(newFilename);
        existingFile >> fileContent;
        existingFile.close();
    }

    // 如果 "components" 不存在，创建一个空数组
    if (!fileContent.isMember("components"))
    {
        fileContent["components"] = Json::Value(Json::arrayValue);
    }

    // 将 modifiedData 对象添加到 components 数组中
    fileContent["components"].append(modifiedData);

    // 将 fileContent 对象写入新文件
    std::ofstream file(newFilename, std::ios::trunc);  // 使用截断模式
    if (file.is_open())
    {
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "    "; // 设置缩进
        std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());

        jsonWriter->write(fileContent, &file);

        file.close();
        std::cout << "成功写入新的JSON文件: " << newFilename << std::endl;
    }
    else
    {
        std::cerr << "无法写入JSON文件: " << newFilename << std::endl;
    }
}

// 提取Json信息
void reWriteJson(const std::string& originalFilename, const std::string& newFilename)
{
    Json::Value jsonData;
    std::ifstream inputFile(originalFilename, std::ifstream::binary);
    inputFile >> jsonData;
    inputFile.close();

    for (Json::Value& component : jsonData["components"])
    {
        Json::Value newComponent;
        newComponent["id"] = component["id"];
        newComponent["name"] = component["name"];
        newComponent["size"] = "size1";

        Json::Value newTextArray;
        for (const Json::Value& text : component["functions"]["text"])
        {
            newTextArray.append(text["content"]);
        }
        newComponent["text"] = newTextArray;
        component = newComponent;
    }

    // Write the modified JSON to a new file
    std::ofstream outputFile(newFilename);
    outputFile << jsonData;
    outputFile.close();
}

void WriteNet(const std::string& originalFilename, const std::string& newFilename) {
    // 读取JSON文件内容
    std::ifstream inputFile(originalFilename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening JSON file: " << originalFilename << std::endl;
        return;
    }

    // 从文件中读取内容到字符串
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string jsonContent = buffer.str();

    // 关闭输入文件
    inputFile.close();

    // 写入修改后的内容到.NET文件
    std::ofstream outputFile(newFilename);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating .NET file: " << newFilename << std::endl;
        return;
    }

    // 在这里你可以对jsonContent进行修改，然后写入到.NET文件
    // 例如，你可以在这里使用字符串替换等方法修改JSON内容

    // 写入修改后的内容到.NET文件
    outputFile << jsonContent;

    // 关闭输出文件
    outputFile.close();

    std::cout << "File successfully converted to .NET: " << newFilename << std::endl;
}



#ifndef POTREEWRITER_H
#define POTREEWRITER_H

#include <string>
#include <thread>
#include <vector>
#include <functional>

#include "AABB.h"
#include "SparseGrid.h"
#include "CloudJS.hpp"
#include "PointAttributes.hpp"

using std::string;
using std::thread;
using std::vector;

namespace Potree{

class PotreeWriter;
class PointReader;
class PointWriter;

//Node八叉树的结点
class PWNode
{
public:
	PWNode(PotreeWriter* potreeWriter, AABB aabb);
	PWNode(PotreeWriter* potreeWriter, int index, AABB aabb, int level);
	~PWNode();
public:
	string name() const; //名称-int类型数字
	float spacing();     //该结点的间距，总间距/2^level

	bool isLeafNode(){return children.size() == 0;}
	bool isInnerNode(){	return children.size() > 0;}

	//从硬盘加载文件读取
	void loadFromDisk();
    
    //Node添加结点
	PWNode *add(Point &point);
	PWNode *createChild(int childIndex);

	void split();

	string workDir(); //输出路径
	string hierarchyPath();//分层路径

	string path();

	void flush();

	//穿过; 横贯
	void traverse(std::function<void(PWNode*)> callback);
	//广度优先
	void traverseBreadthFirst(std::function<void(PWNode*)> callback);
	//获取分层
	vector<PWNode*> getHierarchy(int levels);

	PWNode* findNode(string name);
public:
    int index = -1;
	AABB aabb;
	AABB acceptedAABB; //??
	int level = 0;         
	SparseGrid *grid;      //稀疏网格？
	unsigned int numAccepted = 0;
	PWNode *parent = NULL;
	vector<PWNode*> children;
	bool addedSinceLastFlush = true;
	bool addCalledSinceLastFlush = false;
	PotreeWriter *potreeWriter;
	vector<Point> cache;
	vector<Point> store;
	bool isInMemory = true;
private:
	PointReader *createReader(string path);
	PointWriter *createWriter(string path);
};


class PotreeWriter
{
public:
	AABB aabb;
	AABB tightAABB;
	string workDir;
	float spacing;
	double scale = 0;
	int maxDepth = -1;
	PWNode *root;
	long long numAdded = 0;
	long long numAccepted = 0;
	CloudJS cloudjs;
	OutputFormat outputFormat;   //转出格式
	PointAttributes pointAttributes;//点的属性
	int hierarchyStepSize = 5;  //设置的分层数目？
	vector<Point> store;  //存储点的数组？？
	thread storeThread;  //存储的线程
	int pointsInMemory = 0;
	string projection = "";
	ConversionQuality quality = ConversionQuality::DEFAULT; //转换的质量
	int storeSize = 20'000; //?每个文件2万个点？


	PotreeWriter(string workDir, ConversionQuality quality);

	PotreeWriter(string workDir, AABB aabb, float spacing, int maxDepth, double scale, OutputFormat outputFormat, PointAttributes pointAttributes, ConversionQuality quality);

	~PotreeWriter() { close(); delete root; }

	string getExtension();

	void processStore();

	void waitUntilProcessed();

	void add(Point &p);

	void flush();

	void close(){ flush(); }

	void setProjection(string projection);

	void loadStateFromDisk();

private:

};

}

#endif

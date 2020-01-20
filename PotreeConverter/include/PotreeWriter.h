

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

class PWNode
{
public:
	int index = -1;
	AABB aabb;
	AABB acceptedAABB; //??
	int level = 0;
	SparseGrid *grid;//稀疏网格
	unsigned int numAccepted = 0;
	PWNode *parent = NULL;
	vector<PWNode*> children;
	bool addedSinceLastFlush = true;
	bool addCalledSinceLastFlush = false;
	PotreeWriter *potreeWriter;
	vector<Point> cache;
	vector<Point> store;
	bool isInMemory = true;

	PWNode(PotreeWriter* potreeWriter, AABB aabb);
	PWNode(PotreeWriter* potreeWriter, int index, AABB aabb, int level);
	~PWNode();

	string name() const;

	float spacing();

	bool isLeafNode(){return children.size() == 0;}

	bool isInnerNode(){	return children.size() > 0;}

	void loadFromDisk();

	PWNode *add(Point &point);

	PWNode *createChild(int childIndex);

	void split();

	string workDir();
	//分层路径
	string hierarchyPath();

	string path();

	void flush();

	//穿过; 横贯
	void traverse(std::function<void(PWNode*)> callback);
	//广度优先
	void traverseBreadthFirst(std::function<void(PWNode*)> callback);
	//获取分层
	vector<PWNode*> getHierarchy(int levels);

	PWNode* findNode(string name);
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
	int hierarchyStepSize = 5;  //设置的分层数目
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

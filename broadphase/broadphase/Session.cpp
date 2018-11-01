#include "Session.h"
#include <fstream>
#include <streambuf>
#include "json\json.h"
#include <iostream>
#include "BruteForce.h"
#include "HierarchicalGrid3.h"
#include "SweepAndPrune.h"
#include "tinyxml2.h"
#include "BVH.h"
#include <chrono>

Session::Session() {
	
	algorithms.push_back(new BruteForce);
	algorithms.push_back(new SweepAndPrune);
	algorithms.push_back(new HierarchicalGrid3);
	algorithms.push_back(new BVH);
}


Session::~Session() {
}

void Session::load(string filepath) {
	std::ifstream t(filepath);
	std::string str;
	loadedJsonPath = filepath;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	cout << "loading " << filepath << endl;
	auto jsonObject = (json::Object)json::Deserialize(str);
	cout << "loading complete!" << endl << endl;

	//load startinfo
	try {
		auto startInfo = jsonObject["StartInfo"];

		for (size_t i = 0; i < startInfo.size(); i++) {
			auto first = startInfo[i]["position"];
			auto second = startInfo[i]["size"];
			Vector3 pos(first["x"], first["y"], first["z"]);
			Vector3 size(second["x"], second["y"], second["z"]);
			man.add(new Entity(pos,size,i));
		}
		
	} catch(const runtime_error& e){
		printf("Error encountered: %s\n", e.what());
	}

	//load frame info
	try {
		auto framedata = jsonObject["FrameData"];

		for (size_t i = 0; i < framedata.size(); i++) { //frames
			auto jsonFrame = framedata[i];
			vector<ObjectData*>* frame = new vector<ObjectData*>();
			for (size_t j = 0; j < jsonFrame.size(); j++) { //positions
				auto obj = jsonFrame[j];
				auto vec = obj["position"];
				frame->push_back(new ObjectData(obj["id"].ToInt(), Vector3(vec["x"], vec["y"], vec["z"])));
			}
			frames.push_back(frame);
		}

	} catch (const runtime_error& e) {
		printf("Error encountered: %s\n", e.what());
	}

}

void Session::run() {

	for (auto algoritm : algorithms) {
		cout << "Switched to " << algoritm->getId() << endl;
		man.setBroadPhase(algoritm);
		cout << "init " << algoritm->getId() << endl;
		cout << "resetting positions..." << endl;
		man.setPositions(*frames[0]);
		
		man.initBroadPhase();
		cout << "init complete!" << endl;
		ResultVector resultVector;

		for (size_t i = 0; i < frames.size(); i++) {
			man.setPositions(*frames[i]);
			long long duration = 0;
			duration += man.update();

			auto result = man.doCollision();
			duration += result.first;
			auto collisions = result.second;
			resultVector.push_back(make_pair(duration, collisions));

			//cout << "duration: " << duration << " collisions: " << collisions << endl;
		}
		results.push_back(resultVector);
		cout << "completed " << algoritm->getId() << endl << endl;
	}
	cout << "All algorithms completed!" << endl;
	writeResultsToXml();
	cin.get();
}

void Session::writeResultsToXml() {

	cout << "Writing results to file..." << endl;
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* top = doc->NewElement("data");
	int length = results[0].size();
	for (int i = 0; i < length;i++) {

		tinyxml2::XMLElement* elm = doc->NewElement("frame");
		for (size_t j = 0; j < algorithms.size(); j++) {
			
			elm->SetAttribute(("dur_" + algorithms[j]->getId()).c_str(), results[j][i].first);
			elm->SetAttribute(("col_" + algorithms[j]->getId()).c_str(), results[j][i].second);
			
		}
		top->InsertEndChild(elm);
	}
	doc->InsertEndChild(top);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(loadedJsonPath.c_str(), drive, dir, fname, ext);
	string filename = string(fname)+"_results.xml";
	doc->SaveFile( filename.c_str() );
	cout << "Writing completed!" << endl;



}

/*
void Session::writeResultsToXml() {

	cout << "Writing results to file..." << endl;
	for (size_t i = 0; i < algorithms.size(); i++) {
		tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

		cout << "Writing " << algorithms[i]->getId() << " results" << endl;

		for (auto res : results[i]) {
			tinyxml2::XMLElement* elm = doc->NewElement("frame");
			elm->SetAttribute("duration", res.first);
			elm->SetAttribute("collisions", res.second);
			doc->InsertEndChild(elm);
		}
		
		doc->SaveFile((algorithms[i]->getId() + ".xml").c_str());
		//delete doc;
	}
	cout << "Writing completed!" << endl;

}
*/
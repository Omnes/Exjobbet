#pragma once
#include <iostream>
#include "Entity.h"
#include "EntityManager.h"
#include <string>
#include "Constants.h"
#include "Session.h"

using namespace std;

int main(){

	while (true) {
		Session ses;
		string filepath;
		cout << "Enter the filepath to a json file from the unity simulation." << endl;
		std::cin >> filepath;
		ses.load(filepath);

		ses.run();
		//cin.get();
	}

	return 0;
}
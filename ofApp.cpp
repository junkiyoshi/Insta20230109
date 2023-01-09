#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(2);
	
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	for (int i = 0; i < 2000; i++) {

		this->radius_list.push_back(ofRandom(500));
		this->deg_list.push_back(ofRandom(360));
	}
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	int threshold_radius = 300;

	for (int radius = 0; radius <= 500; radius += 10) {

		int start_index = this->line.getNumVertices();
		int index = start_index;
		int deg_start = radius;
		for (int deg = deg_start; deg < deg_start + 360; deg += 1) {

			auto z = radius > threshold_radius ? 0 : ofMap(radius, 0, threshold_radius, -1 * pow(threshold_radius - radius, 1.1), 0);
			int next_radius = radius + 10;
			int next_z = next_radius > threshold_radius ? 0 : ofMap(next_radius, 0, threshold_radius, -1 * pow(threshold_radius - next_radius, 1.1), 0);

			vector<glm::vec3> vertices;
			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(next_radius * cos((deg + 1) * DEG_TO_RAD), next_radius * sin((deg + 1) * DEG_TO_RAD), next_z));
			vertices.push_back(glm::vec3(next_radius * cos(deg * DEG_TO_RAD), next_radius * sin(deg * DEG_TO_RAD), next_z));

			face.addVertices(vertices);

			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 3);
			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);
		}
	}

	this->line.clear();

	for (int i = 0; i < this->radius_list.size(); i++){

		int speed = 5;
		int radius_start = ((int)(this->radius_list[i] + speed) % 500);
		int radius = 500 - radius_start;
		float deg_start = this->deg_list[i] + (radius > threshold_radius ? 1 : ofMap(radius, 0, threshold_radius, pow(speed, ofMap(radius, 0, threshold_radius, 2, 1)), 1));
		float deg = deg_start;
		int color_value = ofRandom(64, 255);

		for (int k = 0; k < 5; k++) {

			radius = ((int)(this->radius_list[i] + speed * k) % 500);
			radius = 500 - radius;

			deg += (radius > threshold_radius ? 1 : ofMap(radius, 1, threshold_radius, pow(speed, ofMap(radius, 0, threshold_radius, 2, 1)), 1));

			auto z = radius > threshold_radius ? 0 : ofMap(radius, 0, threshold_radius, -1 * pow(threshold_radius - radius, 1.1), 0);
			auto vertex = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z + 2);
			this->line.addVertex(vertex);
			this->line.addColor(ofColor(color_value));

			if (k > 0) {

				this->line.addIndex(this->line.getNumVertices() - 1);
				this->line.addIndex(this->line.getNumVertices() - 2);
			}

		}

		this->radius_list[i] = radius_start;
		this->deg_list[i] = deg_start;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(-35);

	ofSetColor(0);
	this->face.draw();
	ofSetColor(255);
	this->line.drawWireframe();

	this->cam.end();

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
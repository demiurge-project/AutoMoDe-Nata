/**
  * @file <src/modules/AutoMoDeBehaviourGenome2.cpp>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourGenome2.h"


namespace argos {

	int AutoMoDeBehaviourGenome2::INDEX = 0;
	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome2::AutoMoDeBehaviourGenome2(std::string path) {
		genPath = path;
		LoadGen(path);
		m_strLabel = "Genome_"+std::to_string(AutoMoDeBehaviourGenome2::INDEX);
		AutoMoDeBehaviourGenome2::INDEX += 1;
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome2::AutoMoDeBehaviourGenome2(AutoMoDeBehaviourGenome2* pc_behaviour) : AutoMoDeBehaviourGenome2(pc_behaviour->genPath) {
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();
		Init();
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome2::~AutoMoDeBehaviourGenome2() {
		for (UInt8 i = 0; i < m_cNetworkVector.size(); i++) {
    		delete m_cNetworkVector.at(i);
  		}
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome2* AutoMoDeBehaviourGenome2::Clone() {
		return new AutoMoDeBehaviourGenome2(this);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome2::ControlStep() {
		m_pcRobotDAO->SetWheelsVelocity(0,0);
		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome2::Init() {
		//std::cout << GetParameterPath() << std::endl;
	}

	/****************************************/
	/****************************************/

	/*void AutoMoDeBehaviourGenome2::Destroy() {
		//std::cout << GetParameterPath() << std::endl;
	}*/

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome2::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome2::ResumeStep() {
		m_bOperational = true;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome2::LoadGen(std::string filename) {
	    std::ifstream iFile(filename.c_str(),std::ios::in);

		if(iFile) {
			char curword[20];
			int id = 0;

			do {
				iFile >> curword;
				// skip comments in the genome file
				if(strcmp(curword, "/*") == 0) {
					do {
					iFile >> curword;
					} while(strcmp(curword, "*/") != 0);
				}
			} while(strcmp(curword, "genomestart") != 0);

			iFile >> id;
			NEAT::Genome* g = new NEAT::Genome(id,iFile);
			m_net = g->genesis(g->genome_id);
			m_cNetworkVector.push_back(m_net);
			delete g;
		} else {
			THROW_ARGOSEXCEPTION("Cannot open genome file '" << filename << "' for reading");
		}

		iFile.close();
	}
}
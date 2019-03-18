/**
  * @file <src/modules/AutoMoDeBehaviourGenome.cpp>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourGenome.h"


namespace argos {

	int AutoMoDeBehaviourGenome::INDEX = 0;
	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome::AutoMoDeBehaviourGenome() {
		m_strLabel = "Genome_"+std::to_string(AutoMoDeBehaviourGenome::INDEX);
		AutoMoDeBehaviourGenome::INDEX += 1;
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome::AutoMoDeBehaviourGenome(AutoMoDeBehaviourGenome* pc_behaviour) : AutoMoDeBehaviourGenome() {
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();
		Init();
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome::~AutoMoDeBehaviourGenome() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome* AutoMoDeBehaviourGenome::Clone() {
		return new AutoMoDeBehaviourGenome(this);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::ControlStep() {
		m_pcRobotDAO->SetWheelsVelocity(0,0);
		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::Init() {
		std::cout << GetParameterPath() << std::endl;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::ResumeStep() {
		m_bOperational = true;
	}
}

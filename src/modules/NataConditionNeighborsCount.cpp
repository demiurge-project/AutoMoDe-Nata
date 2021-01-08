/**
  * @file <src/modules/NataConditionNeighborsCount.cpp>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

 #include "NataConditionNeighborsCount.h"

 namespace argos {

	/****************************************/
	/****************************************/

	NataConditionNeighborsCount::NataConditionNeighborsCount() {
		m_strLabel = "NeighborsCount";
	}

	/****************************************/
	/****************************************/

	NataConditionNeighborsCount::~NataConditionNeighborsCount() {}

	/****************************************/
	/****************************************/

	NataConditionNeighborsCount::NataConditionNeighborsCount(NataConditionNeighborsCount* pc_condition) {
		m_strLabel = pc_condition->GetLabel();
		m_unIndex = pc_condition->GetIndex();
		m_unIdentifier = pc_condition->GetIndex();
		m_unFromBehaviourIndex = pc_condition->GetOrigin();
		m_unToBehaviourIndex = pc_condition->GetExtremity();
		m_mapParameters = pc_condition->GetParameters();
    Init();
	}

	/****************************************/
	/****************************************/

	NataConditionNeighborsCount* NataConditionNeighborsCount::Clone() {
		return new NataConditionNeighborsCount(this);
	}

	/****************************************/
	/****************************************/

	bool NataConditionNeighborsCount::Verify() {
		UInt32 unNumberNeighbors = m_pcRobotDAO->GetNumberNeighbors();
        if (unNumberNeighbors >= m_fParameterEta) {
		    return EvaluateBernoulliProbability(m_fProbability);
	    }
        return false;
    }

	/****************************************/
	/****************************************/

	void NataConditionNeighborsCount::Reset() {

	}

	/****************************************/
	/****************************************/

	void NataConditionNeighborsCount::Init() {
		std::map<std::string, Real>::iterator itEta = m_mapParameters.find("w");
		std::map<std::string, Real>::iterator itXi = m_mapParameters.find("p");
		if ((itEta != m_mapParameters.end()) && (itXi != m_mapParameters.end())) {
			m_fParameterEta = itEta->second;
			m_fProbability = itXi->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

 }

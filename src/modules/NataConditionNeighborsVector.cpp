/**
  * @file <src/modules/NataConditionNeighborsVector.cpp>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

 #include "NataConditionNeighborsVector.h"

 namespace argos {

	/****************************************/
	/****************************************/

	NataConditionNeighborsVector::NataConditionNeighborsVector() {
		m_strLabel = "NeighborsVector";
	}

	/****************************************/
	/****************************************/

	NataConditionNeighborsVector::~NataConditionNeighborsVector() {}

	/****************************************/
	/****************************************/

	NataConditionNeighborsVector::NataConditionNeighborsVector(NataConditionNeighborsVector* pc_condition) {
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

	NataConditionNeighborsVector* NataConditionNeighborsVector::Clone() {
		return new NataConditionNeighborsVector(this);
	}

	/****************************************/
	/****************************************/

	bool NataConditionNeighborsVector::Verify() {
 
        auto readings = m_pcRobotDAO->GetAttractionVectorToNeighbors(m_fParameterEta);
        //process readings
        if (readings >= m_fThreshold) {
		    return EvaluateBernoulliProbability(m_fProbability);
	    }
        return false;
	}

	/****************************************/
	/****************************************/

	void NataConditionNeighborsVector::Reset() {

	}

	/****************************************/
	/****************************************/

	void NataConditionNeighborsVector::Init() {
		std::map<std::string, Real>::iterator itEta = m_mapParameters.find("w");
		std::map<std::string, Real>::iterator itXi = m_mapParameters.find("p");
		if ((itEta != m_mapParameters.end()) && (itXi != m_mapParameters.end())) {
			m_fParameterEta = itEta->second;
			m_unParameterXi = itXi->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

 }

/**
  * @file <src/modules/NataConditionProx.cpp>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

 #include "NataConditionProx.h"

 namespace argos {

  /****************************************/
  /****************************************/

	NataConditionProx::NataConditionProx() {
		m_strLabel = "Prox";
	}

  /****************************************/
  /****************************************/

	NataConditionProx::~NataConditionProx() {}

  /****************************************/
  /****************************************/

	NataConditionProx::NataConditionProx(NataConditionProx* pc_condition) {
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

  void NataConditionProx::Init() {
	std::map<std::string, Real>::iterator it = m_mapParameters.find("p");
	std::map<std::string, Real>::iterator it2 = m_mapParameters.find("t");
	std::map<std::string, Real>::iterator it3 = m_mapParameters.find("b");
    if (it != m_mapParameters.end() && it2 != m_mapParameters.end() ) {
      m_fProbability = it->second;
      m_fThreshold = it2->second;
      m_unSide = it3->second;
    } else {
      LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
      THROW_ARGOSEXCEPTION("Missing Parameter");
	}
  }

  /****************************************/
  /****************************************/

	NataConditionProx* NataConditionProx::Clone() {
		return new NataConditionProx(this);
	}

  /****************************************/
  /****************************************/

	bool NataConditionProx::Verify() {
        //auto readings = m_pcRobotDAO->GetProximityInput();
	CCI_EPuckProximitySensor::TReadings cProcessedProxiReadings = m_pcRobotDAO->GetProximityInput();
        // process readings
        //auto addReadings = [](Real acc, const auto& obj) { return acc + obj.Value; };
        //Real sum = std::accumulate(readings.begin(), readings.end(), 0.0, addReadings);
	Real sum = 0.0;
	for (int i = 0; i < cProcessedProxiReadings.size(); i++ ) {
	  sum += cProcessedProxiReadings[i].Value;
	}
	

        if (sum >= m_fThreshold and m_unSide == 0) {
          return EvaluateBernoulliProbability(m_fProbability);
        }
        else if (sum <= m_fThreshold and m_unSide == 1){
          return EvaluateBernoulliProbability(m_fProbability);
        } else {
          return false;
        }
	}

  /****************************************/
  /****************************************/

	void NataConditionProx::Reset() {
    Init();
  }

 }

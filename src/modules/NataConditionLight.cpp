/**
  * @file <src/modules/NataConditionLight.cpp>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

 #include "NataConditionLight.h"

 namespace argos {

  /****************************************/
  /****************************************/

	NataConditionLight::NataConditionLight() {
		m_strLabel = "Light";
	}

  /****************************************/
  /****************************************/

	NataConditionLight::~NataConditionLight() {}

  /****************************************/
  /****************************************/

	NataConditionLight::NataConditionLight(NataConditionLight* pc_condition) {
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

  void NataConditionLight::Init() {
	std::map<std::string, Real>::iterator it = m_mapParameters.find("p");
	std::map<std::string, Real>::iterator it2 = m_mapParameters.find("t");
	std::map<std::string, Real>::iterator it3 = m_mapParameters.find("b");
    if (it != m_mapParameters.end() && it2 != m_mapParameters.end() && it3 != m_mapParameters.end()) {
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

	NataConditionLight* NataConditionLight::Clone() {
		return new NataConditionLight(this);
	}

  /****************************************/
  /****************************************/

	bool NataConditionLight::Verify() {
        auto readings = m_pcRobotDAO->GetLightInput();
        //process readings
        auto addReadings = [](Real acc, const auto& obj) { return acc + obj.Value; };
        Real sum = std::accumulate(readings.begin(), readings.end(), 0.0, addReadings);

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

	void NataConditionLight::Reset() {
    Init();
  }

 }

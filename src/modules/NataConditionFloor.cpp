/**
  * @file <src/modules/NataConditionFloor.cpp>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

 #include "NataConditionFloor.h"

 namespace argos {

  /****************************************/
  /****************************************/

	NataConditionFloor::NataConditionFloor() {
		m_strLabel = "Floor";
	}

  /****************************************/
  /****************************************/

	NataConditionFloor::~NataConditionFloor() {}

  /****************************************/
  /****************************************/

	NataConditionFloor::NataConditionFloor(NataConditionFloor* pc_condition) {
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

  void NataConditionFloor::Init() {
	std::map<std::string, Real>::iterator it = m_mapParameters.find("p");
	std::map<std::string, Real>::iterator it2 = m_mapParameters.find("t");
	m_fGroundThresholdRange.Set(0.1, 0.95);
    if (it != m_mapParameters.end() && it2 != m_mapParameters.end() ) {
      m_fProbability = it->second;
      m_fGroundColor = it2->second;

    } else {
      LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
      THROW_ARGOSEXCEPTION("Missing Parameter");
	}
  }

  /****************************************/
  /****************************************/

	NataConditionFloor* NataConditionFloor::Clone() {
		return new NataConditionFloor(this);
	}

  /****************************************/
  /****************************************/

	bool NataConditionFloor::Verify() {
        CCI_EPuckGroundSensor::SReadings readings = m_pcRobotDAO->GetGroundInput();

        if (m_fGroundColor == 0 && (readings.Left <= 0.1 || readings.Center <= 0.1 || readings.Right <= 0.1)) {
          return EvaluateBernoulliProbability(m_fProbability);
        } else if (m_fGroundColor == 1 && (readings.Left >= 0.95 || readings.Center >= 0.95 || readings.Right >= 0.95)) {
            return EvaluateBernoulliProbability(m_fProbability);
        } else if (m_fGroundColor == 2 && (m_fGroundThresholdRange.WithinMinBoundExcludedMaxBoundExcluded(readings.Left) || 
                    m_fGroundThresholdRange.WithinMinBoundExcludedMaxBoundExcluded(readings.Center) || 
                    m_fGroundThresholdRange.WithinMinBoundExcludedMaxBoundExcluded(readings.Right)) ){
            return EvaluateBernoulliProbability(m_fProbability);
        } else {
          return false;
        }

	}

  /****************************************/
  /****************************************/

	void NataConditionFloor::Reset() {
    Init();
  }

 }

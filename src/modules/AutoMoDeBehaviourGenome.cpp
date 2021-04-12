/**
  * @file <src/modules/AutoMoDeBehaviourGenome.cpp>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourGenome.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

	AutoMoDeBehaviourGenome::AutoMoDeBehaviourGenome(std::string str_label, std::string path_to_genome) {
		m_strPathToGenome = path_to_genome;
		LoadGen(path_to_genome);
		m_net = NULL;
		m_nId = -1;
		m_unTimeStep = 0;
		m_mapMessages.clear();
		//m_pcRNG = argos::CRandom::CreateRNG("argos");
		m_strLabel = str_label;
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome::AutoMoDeBehaviourGenome(AutoMoDeBehaviourGenome* pc_behaviour) {
		m_strPathToGenome = pc_behaviour->GetPathToGenome();
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();
		m_strLabel = pc_behaviour->GetLabel();
		Init();
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome::~AutoMoDeBehaviourGenome() {
		for (UInt8 i = 0; i < m_cNetworkVector.size(); i++) {
    		delete m_cNetworkVector.at(i);
  		}
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGenome* AutoMoDeBehaviourGenome::Clone() {
		return new AutoMoDeBehaviourGenome(this);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::ControlStep() {
		for(size_t i=0; i<25; i++) {
			m_inputs[i] = 0;
		}

		CCI_EPuckProximitySensor::TReadings cProcessedProxiReadings = m_pcRobotDAO->GetProximityInput();


		// Injecting processed readings as input of the NN
		// Proximity
		for(size_t i=0; i<8; i++) {
			m_inputs[i] = cProcessedProxiReadings[i].Value;
		}

		// Light
		CCI_EPuckLightSensor::TReadings cProcessedLightReadings = m_pcRobotDAO->GetLightInput();
		for(size_t i=8; i<16; i++) {
			m_inputs[i] = cProcessedLightReadings[i-8].Value;
		}

		// Ground
		CCI_EPuckGroundSensor::SReadings cProcessedGroundReadings = m_pcRobotDAO->GetGroundInput();
		for(size_t i=16; i<19; i++) {
			m_inputs[i] = cProcessedGroundReadings[i-16];
			if(cProcessedGroundReadings[i-16] <= 0.1) { //black
				m_inputs[i] = 0;
			} else if(cProcessedGroundReadings[i-16] >= 0.95){ //white
				m_inputs[i] = 1;
			} else { //gray
				UInt32 index = m_pcRobotDAO->GetRandomNumberGenerator()->Uniform(CRange<UInt32>(0, 4204));
				if(i == 16)
					m_inputs[i] = m_GraySamplesLeft[index];
				else if(i == 17)
					m_inputs[i] = m_GraySamplesCenter[index];
				else
					m_inputs[i] = m_GraySamplesRight[index];
			}
		}

		// Get RAB sensory data.
		CCI_EPuckRangeAndBearingSensor::TPackets sLastPackets = m_pcRobotDAO->GetRangeAndBearingMessages();
        CCI_EPuckRangeAndBearingSensor::SReceivedPacket cProcessedRabReading = m_pcRobotDAO->GetAttractionVectorToNeighbors(1.0);

        CVector2 cRabReading = CVector2(cProcessedRabReading.Range, cProcessedRabReading.Bearing);
        Real len = cRabReading.Length();
        if(len != 0) {
           cRabReading.Normalize(); // now, sRabVectorSum.Length = 1
           cRabReading *= (2/(1+exp(-len)) - 1);
        }

		UInt8 unNumberNeighbors = m_pcRobotDAO->GetNumberNeighbors();

		// Set the RAB input of the NN
		m_inputs[19] = 1 - (2 / (1 + exp(unNumberNeighbors))); // Saturate at 5, and is in [0,1]
		for(int i = 20; i < 24; i++) {
			CRadians cDirection = CRadians::PI*(2*(i-20) + 1)/4;
            Real value = cRabReading.DotProduct(CVector2(1.0, cDirection));
			m_inputs[i] = (value > 0 ? value : 0); // only 2 inputs (rarely 3) will be different from 0.
		}

		// Bias Unit
		m_inputs[24] = 1;

		//std::cout << "Before loading" << std::endl;

		// Feed the network with those inputs
		m_net->load_sensors((double*)m_inputs);
		// Transmit the signals to the next layer.
		m_net->activate();

		// Apply NN outputs to actuation. The NN outputs are in the range [0,1], we remap this range into [-5:5] linearly.
		m_cNeuralNetworkOutputRange.MapValueIntoRange(m_fLeftSpeed, (m_net->outputs[0])->activation, m_cWheelActuationRange);
		m_cNeuralNetworkOutputRange.MapValueIntoRange(m_fRightSpeed, (m_net->outputs[1])->activation, m_cWheelActuationRange);

		m_pcRobotDAO->SetWheelsVelocity(m_fLeftSpeed, m_fRightSpeed);

		m_unTimeStep++;
		m_bLocked = false;

	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::Init() {
		LoadGen(m_strPathToGenome);
		m_cNeuralNetworkOutputRange.Set(0.0f, 1.0f);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::Init(TConfigurationNode& t_node) {
		Init();
	}
	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::Destroy() {
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::Reset() {
		// Reinit the network: Puts the network back into an inactive state
		if (m_net != NULL) {
			m_net->flush();
		}
		m_unTimeStep = 0;
		m_pcRobotDAO->Reset();
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::ResumeStep() {
		m_bOperational = true;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::LoadGen(std::string filename) {
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
			LOGERR << "Cannot open genome file '" << filename << "' for reading";
			THROW_ARGOSEXCEPTION("Cannot open genome file '" << filename << "' for reading");
		}

		iFile.close();
	}

	/****************************************/
	/****************************************/

	UInt32 AutoMoDeBehaviourGenome::getRobotId() {
		if (m_nId < 0) {
			std::string strId = GetId();
			std::string::size_type pos = strId.find_first_of("0123456789");
			std::string numero = strId.substr(pos, strId.size());
			if (!(std::istringstream(numero) >> m_nId)) {
				m_nId = 0;
			}
		}
		return m_nId;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGenome::SetRobotDAO(EpuckDAO* pc_robot_dao) {
		AutoMoDeBehaviour::SetRobotDAO(pc_robot_dao);
		//std::cout << "WheelNull=" << (m_cWheelActuationRange.GetMax()) << std::endl;
		//std::cout << "RobotNull=" << (m_pcRobotDAO!=NULL) << std::endl;
		m_cWheelActuationRange.Set(-m_pcRobotDAO->GetMaxVelocity(), m_pcRobotDAO->GetMaxVelocity());
		//std::cout << "WheelSet=" << (m_cWheelActuationRange.GetMax()) << std::endl;
	}

	/****************************************/
	/****************************************/

	std::string AutoMoDeBehaviourGenome::GetPathToGenome() {
		return m_strPathToGenome;
	}

}

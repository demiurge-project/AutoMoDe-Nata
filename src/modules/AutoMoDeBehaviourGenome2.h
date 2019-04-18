/**
  * @file <src/modules/AutoMoDeBehaviourGenome2.h>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_GENOME2_H
#define AUTOMODE_BEHAVIOUR_GENOME2_H

#include "AutoMoDeBehaviour.h"

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/rng.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>

// ++ modified to relative path (<argos3/demiurge/epuck-dao/EpuckDAO.h>)
#include "../../../demiurge-epuck-dao/src/EpuckDAO.h"
// epuckDAO and NEAT-private must be at same level of directory

#include "../../../demiurge-epuck-dao/src/ReferenceModel1Dot2.h"

#include "../NEAT/network.h"
#include "../NEAT/genome.h"

#include <map>

namespace argos {
	class AutoMoDeBehaviourGenome2: public AutoMoDeBehaviour, public CEPuckNEATController {
		public:
			AutoMoDeBehaviourGenome2();
			AutoMoDeBehaviourGenome2(std::string path);
			AutoMoDeBehaviourGenome2(AutoMoDeBehaviourGenome2* pc_behaviour);
			virtual ~AutoMoDeBehaviourGenome2();

			virtual void ControlStep();
			//virtual void Destroy();// = 0;
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

			virtual AutoMoDeBehaviourGenome2* Clone();

			inline void SetNetwork(NEAT::Network& net) {
      			m_net = &net;
			}

		protected:

			EpuckDAO* m_pcRobotState;

			// Actuators
			CCI_EPuckWheelsActuator* m_pcWheels;
			CCI_EPuckRangeAndBearingActuator* m_pcRABAct;

			CRange<Real> m_cWheelActuationRange;

			// Sensors
			CCI_EPuckProximitySensor* m_pcProximity;
			CCI_EPuckLightSensor* m_pcLight;
			CCI_EPuckGroundSensor* m_pcGround;
			CCI_EPuckRangeAndBearingSensor* m_pcRAB;

			// Network
			NEAT::Network* m_net;
			CRange<Real> m_cNeuralNetworkOutputRange;
			std::vector<NEAT::Network*> m_cNetworkVector;

			// Path to the genome
			std::string m_strFile;
		private:
			virtual void LoadGen(std::string file);
			static int INDEX;
			std::string genPath;
	};
}

#endif

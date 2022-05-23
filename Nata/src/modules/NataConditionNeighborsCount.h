/**
  * @file <src/modules/NataConditionNeighborsCount.h>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

#ifndef NATA_CONDITION_NEIGHBORS_COUNT_H
#define NATA_CONDITION_NEIGHBORS_COUNT_H

#include "AutoMoDeCondition.h"

namespace argos {
	class NataConditionNeighborsCount: public AutoMoDeCondition {
		public:
			NataConditionNeighborsCount();
			virtual ~NataConditionNeighborsCount();

			NataConditionNeighborsCount(NataConditionNeighborsCount* pc_condition);
			virtual NataConditionNeighborsCount* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
			Real m_fParameterEta;
			Real m_fProbability;
            UInt8 m_unSide;
	};
}

#endif

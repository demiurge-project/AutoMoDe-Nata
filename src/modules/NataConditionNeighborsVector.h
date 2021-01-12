/**
  * @file <src/modules/NataConditionNeighborsVector.h>
  *
  * @author Ken H - <khasselm@ulb.ac.be>
  *
  * @package ARGoS3-Nata
  *
  * @license MIT License
  */

#ifndef NATA_CONDITION_NEIGHBORS_VECTOR_H
#define NATA_CONDITION_NEIGHBORS_VECTOR_H

#include "AutoMoDeCondition.h"
namespace argos {
	class NataConditionNeighborsVector: public AutoMoDeCondition{
		public:
			NataConditionNeighborsVector();
			virtual ~NataConditionNeighborsVector();

			NataConditionNeighborsVector(NataConditionNeighborsVector* pc_condition);
			virtual NataConditionNeighborsVector* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
			Real m_fParameterEta;
			UInt8 m_unParameterXi;
            Real m_fProbability;
	};
}

#endif

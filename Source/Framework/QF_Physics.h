#pragma once

namespace QwerkE {

    namespace Physics {

		// #TODO Consider creating a class to handle physics for a Scene
		class PhysicsWorld
		{
		public:
			PhysicsWorld() = default;

			// #TODO Public methods
			void CreateBody () { }
			void CreateCollider() { }
			void CreateTrigger() {}

			void StepSimulation() { }

		private:

		};

        void Initialize();

        void StepSimulation();

        void Shutdown();

    }

}

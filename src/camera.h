#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::mat4 viewMat;
public:
	void lookAt(float targetX, float targetY, float targetZ);
	void moveTo(float posX, float posY, float posZ);
	glm::mat4 getViewMatrix();
};
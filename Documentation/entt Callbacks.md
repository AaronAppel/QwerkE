Examples for callbacks that can come from entt during runtime. Code to refer to in the future.

```
void OnComponentCameraConstruct(entt::registry& registry, entt::entity entity)
{
	LOG_INFO("ComponentCamera created for entity {0}", entity);
}

m_Registry.on_construct<ComponentCamera>().connect<&OnComponentCameraConstruct>();
```
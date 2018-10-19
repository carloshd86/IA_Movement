MOAISim.openWindow("game", 1024, 768)

viewport = MOAIViewport.new()
viewport:setSize (1024, 768)
viewport:setScale (1024, -768)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

-- Entity
texture_name = "dragon.png"
gfxQuad = MOAIGfxQuad2D.new()
gfxQuad:setTexture(texture_name)
char_size = 64
gfxQuad:setRect(-char_size/2, -char_size/2, char_size/2, char_size/2)
gfxQuad:setUVRect(0, 0, 1, 1)
  
prop = MOAIProp2D.new()
prop:setDeck(gfxQuad)

entity = Character.new()
-- Add prop to be the renderable for this character
entity:setProp(prop, layer)
-- Start the character (allow calls to OnUpdate)
entity:start()
entity:setLoc(0, 0)
--entity:setLoc(-400, -200)
entity:setRot(-135)
--entity:setLinearVel(200, 0)
--entity:setAngularVel(0)

-- Enemy
texture_name = "ghost32.png"
gfxQuad = MOAIGfxQuad2D.new()
gfxQuad:setTexture(texture_name)
char_size = 64
gfxQuad:setRect(-char_size/4, -char_size/4, char_size/4, char_size/4)
gfxQuad:setUVRect(0, 0, 1, 1)
  
prop = MOAIProp2D.new()
prop:setDeck(gfxQuad)

enemy = Enemy.new()
-- Add prop to be the renderable for this enemy
enemy:setProp(prop, layer)
-- Start the enemy (allow calls to OnUpdate)
enemy:setTarget(entity)
enemy:start()
enemy:setLoc(0, 0)
enemy:setRot(0)
enemy:setLinearVel(10, 20)
enemy:setAngularVel(0)

-- Enable Debug Draw
debug = MOAIDrawDebug.get();
layer:setDrawDebug(debug)
-- Add entities to draw debug
MOAIDrawDebug.insertEntity(entity)
MOAIDrawDebug.insertEntity(enemy)

mouseX = 0
mouseY = 0

function onClick(down)
  entity:setLoc(mouseX, mouseY)
  entity:setRot(-135)
end

function pointerCallback(x, y)
    mouseX, mouseY = layer:wndToWorld(x, y)
end

MOAIInputMgr.device.mouseLeft:setCallback(onClick)
MOAIInputMgr.device.pointer:setCallback(pointerCallback)



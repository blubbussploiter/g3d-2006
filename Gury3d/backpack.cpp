#include "backpack.h"
#include "GuiRoot.h"


void RBX::Backpack::keypress(UserInput* ui)
{
	G3D::Array<uint16_t> keycodes{};
	RBX::HopperBin* bin;
	std::string keycode;
	char first;
	size_t index;

	ui->pressedKeys(keycodes);
	if (keycodes.size() > 0)
	{
		keycode = ui->keyCodeToString(keycodes.last());
		first = keycode[0];

		if (isdigit(first))
		{
			index = (first - '0');
			if (index > getChildren()->size()) return;
			bin = static_cast<HopperBin*>(getChildren()->at(index-1));

			if (bin)
			{
				if (!items[bin]) return;
				onClickFn(items[bin]);
			}
		}
	}
}

void RBX::Backpack::updateGui(RenderDevice* rd, UserInput* ui)
{
	for (size_t i = 0; i < getChildren()->size(); i++)
	{
		RBX::HopperBin* bin = (RBX::HopperBin*)(getChildren()->at(i));
		if (bin && bin->getClassName() == "HopperBin")
		{
			if (!items[bin])
			{
				items[bin] = createBackpackItem(bin);
			}
			else
			{
				rd->push2D();
				if (items[bin]->number)
				{
					items[bin]->number->title = std::to_string(i + 1);
				}
				items[bin]->position.x = (i * 65);
				items[bin]->handleMouse(ui);
				items[bin]->render(rd);
				rd->popState();
			}
		}
	}
}

RBX::BackpackItem* RBX::Backpack::createBackpackItem(HopperBin* item)
{
	RBX::BackpackItem* n = new RBX::BackpackItem();
	n->fromitem(item);
	return n;
}
#include "QE_Editor.h"

int main(unsigned int argc, char** argv)
{
	const bool supportingEditorReloading = false;
	const bool editorRequestedReload = false;

	do
	{
		QwerkE::Editor::Run(argc, argv);
	}
	while (supportingEditorReloading && editorRequestedReload);

	return 0;
}
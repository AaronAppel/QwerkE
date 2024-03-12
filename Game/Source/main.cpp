
int main(unsigned int argc, char** argv)
{
	// RunBgfxExample(argc, argv, 1600, 900);
	// return 0;

	const bool supportingEditorReloading = false;
	const bool editorRequestedReload = false;

	do
	{
		QwerkE::Editor::Run(argc, argv);
	} while (supportingEditorReloading && editorRequestedReload);

	return 0;
}
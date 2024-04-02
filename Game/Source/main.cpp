
int main(unsigned int argc, char** argv)
{
	const bool supportingEditorReloading = false; // #FEATURE
	const bool editorRequestedReload = false; // #FEATURE

	do
	{
		QwerkE::Editor::Run(argc, argv);
	} while (editorRequestedReload && supportingEditorReloading);

	return 0;
}
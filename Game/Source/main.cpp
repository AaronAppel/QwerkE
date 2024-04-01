
int main(unsigned int argc, char** argv)
{
	const bool supportingEditorReloading = false;
	const bool editorRequestedReload = false;

	do
	{
		QwerkE::Editor::Run(argc, argv);
	} while (editorRequestedReload && supportingEditorReloading);

	return 0;
}
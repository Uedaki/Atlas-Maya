#pragma once

#include <maya/MPxCommand.h>

class MSyntax;

class RenderProcedure : public MPxCommand
{
public:
	static const char *name;
	static void *creator();
	static MSyntax createSyntax();

	~RenderProcedure() override = default;

	MStatus doIt(const MArgList &arg) override;

	bool isUndoable() const override { return (false); }
	bool hasSyntax() const override { return (true); }
private:
};
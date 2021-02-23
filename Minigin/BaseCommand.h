#pragma once
namespace engine
{
	class BaseCommand
	{
	public:
		virtual ~BaseCommand() = default;
		virtual void Execute() = 0;

	private:

	};
}
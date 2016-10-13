
// the first node should be global namespaces.
// if without { should be discard
// lambda with type without name""
// Rules: PreprocStatement, Declaration, Executable
// just display the scope

//void Parser::specifyaddRule(IRule* pRule)

namespace specifyNamespace{
	class TESTClass {
	public:
		void func1{
			operator[] () { }
			operator== () { }
			operator+= () { }
			operator= () { }
		}
		auto p = vglambda([](auto v1, auto v2, auto v3) { std::cout << v1 << v2 << v3; });
		auto vglambda = [](auto printer) {
			return [=](auto&&... ts) // generic lambda, ts is a parameter pack
			{
				printer(std::forward<decltype(ts)>(ts)...);
			};
		};
		func2() {
			try
			{
				if (true)
				{

				}
				else
				{

				}
			}
			catch (const std::exception&)
			{
				do
				{

				} while (true);

			}
		}
	};
}




